///        Copyright 2016 libo. All rights reserved
///   (Home at https://github.com/NingLeixueR/middleware/)

#ifndef READ_CONFIG_H
#define READ_CONFIG_H

#include <boost/thread.hpp>

#include <cstdint>
#include <unordered_map>
#include <map>
#include <fstream>

namespace middleware {
  namespace tools {
    /*
      读取csv文件
    */
#define CSV_SEGMENTATION_CHAR     ( ',' )       /* csv分割字符 */


    class table
    {
      uint32_t milen;
      uint32_t mikey;
      std::vector<char*> modata_vec;
      static char m_segmentation;

      bool copy_to_vec(char* aiData)
      {
        modata_vec.push_back(aiData);
        sscanf(aiData, "%d", &mikey);
        milen = strlen(aiData);
        for (uint32_t i = 0; i < milen; i++)
        {
          if (aiData[i] == m_segmentation)
          {
            aiData[i] = '\0';
            modata_vec.push_back(&(aiData[i + 1]));
          }
        }
        return true;
      }
    public:
      table()
      {
      }

      table(int aikey) :mikey(aikey)
      {

      }

      void set_segmentation(const char aisegm)
      {
        m_segmentation = aisegm;
      }

      uint32_t set_data(char* aiData)
      {
        if (NULL != aiData)
        {
          if (copy_to_vec(aiData))
          {
            return mikey;
          }
          else
          {
            return (uint32_t)(-1);
          }
        }
        else
        {
          throw 0;
        }
      }

      const char* operator[](unsigned int arr) const
      {
        if (arr < modata_vec.size() && arr >= 0)
        {
          return modata_vec[arr];
        }
        else
        {
          return NULL;
        }
      }

      bool operator<(const table& r)const
      {
        return mikey < r.mikey;
      }

      bool operator==(const table& r)const
      {
        return mikey == r.mikey;
      }

      void clear()
      {
        modata_vec.clear();
        milen = 0;
        mikey = 0;
      }

      ~table() {};
    };


    /*
      csv格式
      前五行保留 系统使用   包括以下
      一.第一行指定一系列配置信息
        1.第一个格 csv所采用的分割符号  默认使用','  分割符号不能出现在数据中
      二.第二行 各列数据表示的含义
    */
    class tabkey_config;

    typedef std::unordered_map< uint32_t, table* > ump_tabkey;
    typedef std::unordered_map< uint32_t, tabkey_config* > ump_fkey;

    class tabkey_config
    {
      void open_file(const char* aifilename)
      {
        m_readfile.open(aifilename);
        if (!m_readfile.is_open())
        {
          m_readfile.close();
          throw 0;
        }
      }
      void close_file()
      {
        m_readfile.close();
      }

      void read_file()
      {
        //获取文件大小 
        m_readfile.seekg(0, std::ios::end);
        size_t lisize = (size_t)(m_readfile.tellg());
        m_readfile.seekg(std::ios::beg);
        char* lcbuf = new char[lisize + 1];
        m_p = lcbuf;
        memset(lcbuf, 0x0, lisize);

        m_readfile.read(lcbuf, lisize);
        lcbuf[lisize] = '\n';

        table* litab = NULL;
        char* p = lcbuf;
        uint32_t temp_key = 0;
        bool lbo = false;
        for (size_t i = 0; i <= lisize; ++i)
        {
          if (lcbuf[i] == '\n' || i == lisize)
          {
            lcbuf[i] = 0;

            if (*p == 0)//过滤空行
            {
              p = &(lcbuf[i + 1]);
              continue;
            }

            if (!lbo)/* 过滤表头 */
            {
              lbo = true;
              p = &(lcbuf[i + 1]);
              continue;
            }

            if (litab == NULL)
            {
              litab = new table();
            }

            temp_key = litab->set_data(p);
            if (temp_key == (uint32_t)(-1))
            {
              p = &(lcbuf[i + 1]);
              continue;
            }
            else
            {
              m_tabkey.insert(std::make_pair(temp_key, litab));
              litab = NULL;
            }

            p = &(lcbuf[i + 1]);

          }
        }

        if (litab != NULL)
        {
          delete litab;
        }



      }
      ump_tabkey m_tabkey;
      static std::ifstream m_readfile;
      static boost::mutex m_lock;
      char* m_p;
    public:
      tabkey_config(const char* apfkey) :
        m_p(NULL)
      {
        m_lock.lock();
        try
        {
          open_file(apfkey);
          read_file();
          m_readfile.close();
        }
        catch (...)
        {
          std::cout << " tabkey_config(const char*) error " << std::endl;
        }


        m_lock.unlock();
      }


      table& operator[](uint32_t aikey)
      {
        auto itor = m_tabkey.find(aikey);
        if (itor != m_tabkey.end())
        {
          return *(itor->second);
        }
        else
        {
          throw 0;
        }
      }


      void for_each(boost::function< void(std::pair< const uint32_t, table* >) > aifun)
      {
        for (auto itor = m_tabkey.begin(); itor != m_tabkey.end(); ++itor)
        {
          aifun(*itor);
        }
      }
      ~tabkey_config()
      {
        if (m_p == NULL)
        {
          delete m_p;
        }

        for (auto itor = m_tabkey.begin(); itor != m_tabkey.end(); ++itor)
        {
          delete itor->second;
        }

        m_tabkey.clear();
      }
    };


    class fkey_config
    {


      ump_fkey m_fkey;
    public:


      /*
        通过apfkey 读取配置文件件  返回uint32_t key
      */
      void push(const char* apfile, uint32_t aikey)
      {
        auto itor = m_fkey.find(aikey);
        if (itor != m_fkey.end())
        {
          return;
        }

        tabkey_config* p = new tabkey_config(apfile);
        m_fkey.insert(std::make_pair(aikey, p));
        return;
      }




      tabkey_config& operator [](uint32_t afkey)
      {
        auto itor = m_fkey.find(afkey);
        if (itor != m_fkey.end())
        {
          return *(itor->second);
        }
        else
        {
          throw 0;
        }
      }

      void for_each(boost::function< void(std::pair< const uint32_t, tabkey_config* >) > aifun)
      {
        for (auto itor = m_fkey.begin(); itor != m_fkey.end(); ++itor)
        {
          aifun(*itor);
        }
      }

    };

  }  //namespace tools
}  //namespace middleware 

#endif //READ_CONFIG_H
/* vim: set expandtab ts=2 sw=2 sts=2 tw=100: */