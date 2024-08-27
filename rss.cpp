#include "tinyxml2.h"
#include <cerrno>
#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <fstream>

using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::cerr;
using std::regex;
using std::ofstream;
using namespace tinyxml2;

struct RssItem
{
    string title;
    string link;
    string description;
    string content;
};

class RssReader
{
public:
    RssReader(){};
    void parseRss();//解析
    void dump(const string & filename);//输出
private:
    vector<RssItem> _rss;
 };  

//RssReader::RssReader()

void RssReader::parseRss(){
    XMLDocument doc;
    doc.LoadFile("coolshell.xml");
    if(doc.ErrorID()){
        cerr << "Error: " << doc.ErrorName() << " " << doc.ErrorStr() << endl;
        return ;
    }

    XMLElement* itemNode = doc.FirstChildElement( "rss" )->FirstChildElement( "channel" )->FirstChildElement( "item" );//底层是个链表
    
    while(itemNode){ //如果item存在
        string title = itemNode->FirstChildElement( "title" )->GetText();
        string link = itemNode->FirstChildElement( "link" )->GetText();
        string description = itemNode->FirstChildElement( "description" )->GetText();   
        string content = itemNode->FirstChildElement( "content:encoded" )->GetText();
        //接下来开始对description和content的标签进行过滤
        std::regex htmlTagRegex("<[^>]*>");
        description = std::regex_replace(description, htmlTagRegex, "");
        content = std::regex_replace(content, htmlTagRegex, "");

        _rss.push_back({title, link, description, content});

        itemNode = itemNode->NextSiblingElement();
        //cout << title << endl;
        //cout << link << endl;
        //cout << description << endl;
        //cout<<"======================================================="<<endl;
        //cout << content << endl;
    }
    return ;
}

void RssReader::dump(const string & filename){
    //遍历容器将文本保存到另一个文件中
    ofstream ofs(filename);
    int count = 0;
    //使用迭代器遍历vector并写入所有字符内容
    for (vector<RssItem>::iterator it = _rss.begin(); it!= _rss.end(); it++)
    {
        /* code */
        ofs<<"<doc>"<<endl<<
            "<docid>"<<count<<"</docid>"<<endl<<
            "<title>"<<(*it).title<<"</title>"<<endl<<
            "<link>"<<(*it).link<<"</link>"<<endl<<
            "<description>"<<(*it).description<<"</description>"<<endl<<
            "<content>"<<(*it).content<<"</content>"<<endl
           <<"</doc>"<<endl;
        count++;
    }
    
    ofs.close();
}

int main(void)
{
    RssReader rss;
    rss.parseRss();
    rss.dump("pagelib.dat");
    return 0;
}

