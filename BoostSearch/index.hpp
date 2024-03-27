#include <iostream>
#include <string>
#include <vector>
#include <mutex>
#include <unordered_map>
#include <boost/filesystem.hpp>
#include <boost/algorithm/string.hpp>
#include "cppjieba/Jieba.hpp"
#include "util.hpp"

using namespace std;

namespace Indexing
{
    struct Document
    {
        string title;
        string content;
        string url;
    };

    struct ForwardIndexElement
    {
        string title;
        string url;
    };

    struct InvertedIndexElement
    {
        string word;
        unordered_map<int, int> doc_ids_weights; // 文档id到权重的映射
    };

    class Index
    {
    private:
        static unique_ptr<Index> instance;
        static mutex mutex_instance; // 互斥锁

        Index() {}
        ~Index() = default;

    public:
        vector<Document> documents;
        vector<ForwardIndexElement> forward_index;
        unordered_map<string, InvertedIndexElement> inverted_index;

        static Index *getInstance()
        {
            if (!instance)
            {
                std::lock_guard<std::mutex> lock(mutex_instance);

                if (!instance)
                {                                // 双重检查
                    instance.reset(new Index()); // 创建实例
                }
            }
            return instance.get();
        }
        // 删除拷贝构造函数和拷贝赋值运算符，防止复制实例
        Index(const Index &) = delete;
        void operator=(const Index &) = delete;

        void addDocument(const Document &doc)
        {
            int doc_id = documents.size();
            documents.push_back(move(doc));
            ForwardIndexElement forward_element = {doc.title, doc.url};
            forward_index.push_back(move(forward_element));

            // 分词并统计词频
            vector<string> title_words = tokenize(doc.title);
            vector<string> content_words = tokenize(doc.content);

            updateInvertedIndex(doc_id, title_words);
            updateInvertedIndex(doc_id, content_words);
        }

        void updateInvertedIndex(int doc_id, const vector<string> &words)
        {
            // 计算权重：直接使用词频作为权重
            unordered_map<string, int> word_freq;
            for (const string &word : words)
            {
                // 将词转换为小写
                string lowercase_word = boost::to_lower_copy(word);
                word_freq[lowercase_word]++;
            }

            for (const auto &pair : word_freq)
            {
                inverted_index[pair.first].word = pair.first;
                inverted_index[pair.first].doc_ids_weights[doc_id] = pair.second;
            }
        }

        vector<string> tokenize(const string &text)
        {
            vector<string> words;
            Util::JiebaUtil *jieba;
            jieba = Util::JiebaUtil::getInstance();
            (*jieba).cutForSearch(text, words);
            return words;
        }

        void buildIndex(const string &input)
        {
            ifstream infile(input);
            if (!infile.is_open())
            {
                cerr << "Error: Unable to open input file." << endl;
                return;
            }

            string line;
            while (getline(infile, line))
            {
                vector<string> tokens;
                boost::split(tokens, line, boost::is_any_of("\3"), boost::token_compress_on);

                if (tokens.size() != 3)
                {
                    cerr << "Error: Insufficient fields in input line." << endl;
                    continue;
                }

                Document doc;
                doc.title = tokens[0];
                doc.content = tokens[1];
                doc.url = tokens[2];
                addDocument(doc);

                // stringstream in(line);
                // Document doc;
                // getline(in, doc.title, '\3');
                // getline(in, doc.content, '\3');
                // getline(in, doc.url, '\3');
                // index.addDocument(doc);
            }

            infile.close();
        }
    };
}