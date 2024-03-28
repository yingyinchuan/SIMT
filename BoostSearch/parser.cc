#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <boost/filesystem.hpp>

using namespace std;
namespace fs = boost::filesystem;

struct Document
{
    string title;
    string content;
    string url;
};

string read_file(const fs::path &file_path)
{
    string content;
    if (!fs::exists(file_path) || !fs::is_regular_file(file_path))
    {
        cerr << "Error: Unable to open input file: " << file_path << endl;
        return content;
    }

    ifstream file(file_path.string());
    if (!file.is_open())
    {
        cerr << "Error: Unable to open input file: " << file_path << endl;
        return content;
    }

    // Read file content into a string
    content = string((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

    file.close();

    return content;
}

string remove_html_tags(const string &html)
{
    string result;
    bool in_tag = false;
    for (char c : html)
    {
        if (c == '<')
        {
            in_tag = true;
        }
        else if (c == '>')
        {
            in_tag = false;
            continue;
        }
        else if (c == '\n')
        {
            continue;
        }
        if (!in_tag)
        {
            result += c;
        }
    }
    return result;
}

string parse_title(const string &html)
{
    string title;
    size_t title_start = html.find("<title>");
    size_t title_end = html.find("</title>");
    if (title_start != string::npos && title_end != string::npos)
    {
        title = html.substr(title_start + 7, title_end - title_start - 7);
    }
    return title;
}

string build_url(const string &base_url, const string &file_name)
{
    return base_url + file_name;
}

Document parse_document(const fs::path &file_path)
{
    Document doc;

    string html_content = read_file(file_path);
    if (html_content.empty())
    {
        return doc;
    }

    doc.title = parse_title(html_content);
    doc.content = remove_html_tags(html_content);

    doc.url = build_url("https://www.boost.org/doc/libs/1_84_0/doc/html/", file_path.filename().string());

    return doc;
}

static void ShowDoc(const Document &doc)
{
    cout << "title: " << doc.title << endl;
    cout << "content: " << doc.content << endl;
    cout << "url: " << doc.url << endl;
}

void parse_html_files(const string &input_dir, const string &output_file)
{
    ofstream outfile(output_file, ios::binary);
    if (!outfile.is_open())
    {
        cerr << "Error: Unable to open output file." << endl;
        return;
    }

    const char sep = '\3';
    for (const auto &entry : fs::directory_iterator(input_dir))
    {
        if (fs::is_regular_file(entry) && entry.path().extension() == ".html")
        {
            Document doc = parse_document(entry.path());
            if (!doc.title.empty() && !doc.content.empty())
            {
                string out_string = doc.title + sep + doc.content + sep + doc.url + '\n';
                outfile.write(out_string.c_str(), out_string.size());
            }
        }
    }

    cout << "Parsing completed successfully." << endl;
}

int main()
{
    parse_html_files("data/input/", "data/output/raw");
    return 0;
}