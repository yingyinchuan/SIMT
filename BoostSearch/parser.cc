#include <iostream>
#include <string>
#include <boost/filesystem.hpp>

using namespace std;
namespace fs = boost::filesystem;

// 结构体用于组织标题、内容和URL
struct Document
{
    string title;
    string content;
    string url;
};

// 读取文件内容到字符串
string read_file(const fs::path &file_path)
{
    string content;
    if (!fs::exists(file_path) || !fs::is_regular_file(file_path))
    {
        cerr << "Error: Unable to open input file: " << file_path << endl;
        return content;
    }

    // 打开文件
    fs::ifstream file(file_path);
    if (!file.is_open())
    {
        cerr << "Error: Unable to open input file: " << file_path << endl;
        return content;
    }

    // 获取文件大小
    std::streampos fileSize;
    fileSize = fs::file_size(file_path);

    // 读取文件内容
    content.resize(fileSize);
    file.read(&content[0], fileSize);

    // 关闭文件
    file.close();

    return content;
}

// 清洗 HTML 标签
string remove_html_tags(const string &html)
{
    string result;
    // 状态机
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
        if (!in_tag)
        {
            result += c;
        }
    }
    return result;
}

// 解析标题
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

// 构建URL
string build_url(const string &base_url, const string &file_name)
{
    return base_url + file_name;
}

// 解析文档并返回结构体
Document parse_document(const fs::path &file_path)
{
    Document doc;

    // 读取文件内容
    string html_content = read_file(file_path);
    if (html_content.empty())
    {
        return doc;
    }

    // 解析标题和内容
    doc.title = parse_title(html_content);
    doc.content = remove_html_tags(html_content);

    // 构建URL
    doc.url = build_url("https://www.boost.org/doc/libs/1_84_0/doc/html/", file_path.filename().string());

    return doc;
}

static void ShowDoc(const Document &doc)
{
    cout << "title: " << doc.title << endl;
    cout << "content: " << doc.content << endl;
    cout << "url: " << doc.url << endl;
}

// 解析 HTML 文件并写入到输出文件
void parse_html_files(const string &input_dir, const string &output_file)
{
    ofstream outfile(output_file);
    if (!outfile.is_open())
    {
        cerr << "Error: Unable to open output file." << endl;
        return;
    }

    // int i = 0;

    for (const auto &entry : fs::directory_iterator(input_dir))
    {
        if (fs::is_regular_file(entry) && entry.path().extension() == ".html")
        {
            Document doc = parse_document(entry.path());
            if (!doc.title.empty() && !doc.content.empty())
            {
                outfile << doc.title << "\\3" << doc.content << "\\3" << doc.url << '\n';
                // if (i++ < 10)
                //     ShowDoc(doc);
            }
        }
    }

    // 关闭输出文件
    outfile.close();

    cout << "Parsing completed successfully." << endl;
}

int main()
{
    parse_html_files("data/input/", "data/output/raw.txt");
    return 0;
}