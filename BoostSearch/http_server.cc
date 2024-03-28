#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include "searcher.hpp"

using namespace std;
using namespace boost::asio;
using namespace boost::asio::ip;

class HTTPServer
{
private:
    io_service &io;
    tcp::acceptor acceptor;
    Searching::Searcher searcher;

public:
    HTTPServer(io_service &io_service, short port, const string &index_input)
        : io(io_service), acceptor(io_service, tcp::endpoint(tcp::v4(), port)), searcher()
    {
        searcher.InitSearcher(index_input);
        start_accept();
    }

private:
    void start_accept()
    {
        tcp::socket socket(io);
        acceptor.accept(socket);

        string query;
        boost::system::error_code ec;

        // Read request
        boost::asio::streambuf request;
        boost::asio::read_until(socket, request, "\r\n\r\n", ec);
        if (!ec)
        {
            istream request_stream(&request);
            getline(request_stream, query); // Assume query is in the first line of the HTTP request
        }

        // Process query
        string json_response;
        searcher.Search(query, json_response);

        // Send response
        string response = "HTTP/1.1 200 OK\r\n";
        response += "Content-Length: " + to_string(json_response.length()) + "\r\n";
        response += "Content-Type: application/json\r\n\r\n";
        response += json_response;

        boost::asio::write(socket, boost::asio::buffer(response), ec);

        if (ec)
        {
            cerr << "Error writing response: " << ec.message() << endl;
        }

        socket.close();

        start_accept();
    }
};

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        cerr << "Usage: " << argv[0] << " <port> <index_input>" << endl;
        return 1;
    }

    try
    {
        boost::asio::io_service io_service;
        HTTPServer server(io_service, stoi(argv[1]), argv[2]);
        io_service.run();
    }
    catch (exception &e)
    {
        cerr << "Exception: " << e.what() << endl;
    }

    return 0;
}
