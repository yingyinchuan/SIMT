#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <mutex>
#include <unordered_map>
#include <boost/algorithm/string.hpp>
// #include "log.hpp"
#include "cppjieba/Jieba.hpp"

using namespace std;

const char *const DICT_PATH = "./dict/jieba.dict.utf8";
const char *const HMM_PATH = "./dict/hmm_model.utf8";
const char *const USER_DICT_PATH = "./dict/user.dict.utf8";
const char *const IDF_PATH = "./dict/idf.utf8";
const char *const STOP_WORD_PATH = "./dict/stop_words.utf8";

namespace Util
{
    class JiebaUtil
    {
    private:
        cppjieba::Jieba jieba;
        static unique_ptr<JiebaUtil> instance;
        static mutex mutex_instance; // 互斥锁

        // 私有构造函数，防止外部创建实例
        JiebaUtil()
            : jieba(DICT_PATH, HMM_PATH, USER_DICT_PATH, IDF_PATH, STOP_WORD_PATH)
        {
        }
        ~JiebaUtil() = default;

    public:
        // 获取单例实例的静态方法
        static JiebaUtil *getInstance()
        {
            if (!instance)
            {
                lock_guard<mutex> lock(mutex_instance); // 加锁

                if (!instance)
                {
                    instance.reset(new JiebaUtil());
                }
            }
            return instance.get();
        }

        // 删除拷贝构造函数和拷贝赋值运算符，防止复制实例
        JiebaUtil(const JiebaUtil &) = delete;
        void operator=(const JiebaUtil &) = delete;

        void cutForSearch(const string &text, vector<string> &words)
        {
            jieba.CutForSearch(text, words);
        }
    };
}
