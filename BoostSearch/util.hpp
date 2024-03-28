#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <mutex>
#include <unordered_map>
#include <boost/algorithm/string.hpp>
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
        static JiebaUtil *instance;
        static mutex mutex_instance;

        JiebaUtil()
            : jieba(DICT_PATH, HMM_PATH, USER_DICT_PATH, IDF_PATH, STOP_WORD_PATH)
        {
        }

    public:
        ~JiebaUtil()
        {
            delete instance;
        }

        static JiebaUtil *getInstance()
        {
            if (!instance)
            {
                lock_guard<mutex> lock(mutex_instance);

                if (!instance)
                {
                    instance = new JiebaUtil();
                }
            }
            return instance;
        }

        JiebaUtil(const JiebaUtil &) = delete;
        void operator=(const JiebaUtil &) = delete;

        void cutForSearch(const string &text, vector<string> &words)
        {
            jieba.CutForSearch(text, words);
        }
    };

    JiebaUtil *JiebaUtil::instance = nullptr;
    mutex JiebaUtil::mutex_instance;
}
