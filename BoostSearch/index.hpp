#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <mutex>
#include <unordered_map>
#include <boost/filesystem.hpp>
#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/algorithm/string.hpp>
#include "cppjieba/Jieba.hpp"
#include "util.hpp"

using namespace std;
namespace fs = boost::filesystem;
namespace io = boost::iostreams;

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
        unordered_map<int, int> doc_ids_weights;
    };

    class Index
    {
    private:
        static Index *instance;
        static mutex mutex_instance;

        Index() {}

    public:
        vector<Document> documents;
        vector<ForwardIndexElement> forward_index;
        unordered_map<string, InvertedIndexElement> inverted_index;

        ~Index()
        {
            delete instance;
        }

        static Index *getInstance()
        {
            if (!instance)
            {
                lock_guard<mutex> lock(mutex_instance);

                if (!instance)
                {
                    instance = new Index();
                }
            }
            return instance;
        }
        Index(const Index &) = delete;
        void operator=(const Index &) = delete;

        void addDocument(const Document &doc)
        {
            int doc_id = documents.size();
            documents.push_back(move(doc));
            ForwardIndexElement forward_element = {doc.title, doc.url};
            forward_index.push_back(move(forward_element));

            vector<string> title_words = tokenize(doc.title);
            vector<string> content_words = tokenize(doc.content);

            updateInvertedIndex(doc_id, title_words);
            updateInvertedIndex(doc_id, content_words);
        }

        void updateInvertedIndex(int doc_id, const vector<string> &words)
        {
            unordered_map<string, int> word_freq;
            for (const string &word : words)
            {
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
            io::stream<io::file_source> infile(input, ios_base::in | ios_base::binary);
            if (!infile.is_open())
            {
                cerr << "Error: Unable to open input file." << endl;
                return;
            }

            const char sep = '\3';
            for (string line; getline(infile, line);)
            {
                stringstream ss(line);
                Document doc;
                if (!(getline(ss, doc.title, sep) &&
                      getline(ss, doc.content, sep) &&
                      getline(ss, doc.url)))
                {
                    cerr << "Error: Insufficient fields in input line." << endl;
                    continue;
                }

                addDocument(doc);
            }

            cout << "Index building completed successfully." << endl;
        }
    };
    Index *Index::instance = nullptr;
    mutex Index::mutex_instance;
}