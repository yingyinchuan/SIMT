#pragma once

#include "index.hpp"
#include "util.hpp"
#include <algorithm>
#include <unordered_map>
#include <jsoncpp/json/json.h>

using namespace std;

namespace Searching
{
    class Searcher
    {
    private:
        Indexing::Index *index;

    public:
        Searcher() : index(nullptr) {}
        ~Searcher() {}

        void InitSearcher(const string &input)
        {
            index = Indexing::Index::getInstance();
            index->buildIndex(input);
        }

        void Search(const string &query, string &json_string)
        {
            vector<string> query_tokens;

            boost::split(query_tokens, query,
                         boost::is_any_of(" "),
                         boost::token_compress_on);

            unordered_map<int, int> results;
            for (const string &token : query_tokens)
            {
                string lowercase_token = boost::to_lower_copy(token);
                if (index->inverted_index.find(lowercase_token) != index->inverted_index.end())
                {
                    const unordered_map<int, int> &doc_ids_weights{index->inverted_index[lowercase_token].doc_ids_weights};
                    for (const auto &pair : doc_ids_weights)
                    {
                        int doc_id = pair.first;
                        int weight = pair.second;
                        results[doc_id] += weight;
                    }
                }
            }

            vector<pair<int, int>> sorted_results;
            for (const auto &pair : results)
            {
                sorted_results.push_back(pair);
            }
            sort(sorted_results.begin(), sorted_results.end(),
                 [](const pair<int, int> &a, const pair<int, int> &b)
                 {
                     return a.second > b.second;
                 });

            Json::Value root;
            Json::Value documents(Json::arrayValue);
            for (const auto &result : sorted_results)
            {
                int doc_id = result.first;
                const Indexing::Document &doc = index->documents[doc_id];
                Json::Value document;
                document["title"] = doc.title;
                document["summary"] = getSummary(doc.content);
                document["url"] = doc.url;
                document["doc_id"] = doc_id;
                documents.append(document);
            }
            root["results"] = documents;
            Json::StreamWriterBuilder writer;
            json_string = Json::writeString(writer, root);
        }

        string getSummary(const string &content)
        {
            const int max_summary_length = 100;
            if (content.length() <= max_summary_length)
            {
                return content;
            }
            else
            {
                return content.substr(0, max_summary_length) + "...";
            }
        }
    };
}
