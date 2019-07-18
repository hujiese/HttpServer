#include <http/HttpServer.h>
#include <http/HttpRequest.h>
#include <http/HttpResponse.h>
#include <net/EventLoop.h>
#include <base/Logging.h>
#include <iostream>
#include <fstream>
#include<string>
#include <map>

using namespace muduo;
using namespace muduo::net;

bool benchmark = false;
string staticFilePath = "/home/jack/workspace/httpserver/static"; 

string readFileContent(string file)
{
    string content = "";
    std::ifstream infile; 
    infile.open(file.data());   //将文件流对象与文件连接起来 
    //assert(infile.is_open());   //若失败,则输出错误消息,并终止程序运行 

    string s;
    while(getline(infile,s))
    {
        content += s + "\r\n";
    }
    infile.close();             //关闭文件输入流 

    return content;
}


// 实际的请求处理
void onRequest(const HttpRequest& req, HttpResponse* resp)
{
  std::cout << "Headers " << req.methodString() << " " << req.path() << std::endl;
  if (!benchmark)
  {
    const std::map<string, string>& headers = req.headers();
    for (std::map<string, string>::const_iterator it = headers.begin();
         it != headers.end();
         ++it)
    {
      std::cout << it->first << ": " << it->second << std::endl;
    }
  }

  string resPath = staticFilePath + req.path();
  if (req.path() == "/")
  {
    resPath += "index.html";
  }

  std::ifstream fileIsExist(resPath.data());
  if(!fileIsExist)
  {
    resp->setStatusCode(HttpResponse::k404NotFound);
    resp->setStatusMessage("Not Found");
    resp->setCloseConnection(true);
  }
  else
  {
      string image = "images";
      if(strstr(resPath.c_str(),image.c_str()))
      {
	    FILE * f = fopen(resPath.data(),"rb");
	    if (f)
	    {
			fseek(f,0,SEEK_END);
			long size = ftell(f);
			char buf[size];
			fseek(f,0,SEEK_SET);
			memset(buf,0,size);
			int nRead = fread(buf,sizeof(char),size,f);
			fclose(f);
			resp->setStatusCode(HttpResponse::k200Ok);
			resp->setStatusMessage("OK");
			resp->addHeader("Server", "Jackster");
			resp->setBody(string(buf, sizeof buf));
	   }
      }
      else
      {
			string content = readFileContent(resPath);
			resp->setStatusCode(HttpResponse::k200Ok);
			resp->setStatusMessage("OK");
			//resp->setContentType("text/html");
			resp->addHeader("Server", "Jackster");
			resp->setBody(content);
      }
  }
  fileIsExist.close();
}

int main(int argc, char* argv[])
{
  int numThreads = 0;
  if (argc > 1)
  {
    benchmark = true;
    Logger::setLogLevel(Logger::WARN);
    numThreads = atoi(argv[1]);
  }
  EventLoop loop;
  HttpServer server(&loop, InetAddress(8000), "Jackster");
  server.setHttpCallback(onRequest);
  server.setThreadNum(numThreads);
  server.start();
  loop.loop();
}