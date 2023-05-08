#include <iostream>
#include <memory>
#include <cstring>
#include <vector>
#include <string>

#include <curl/curl.h>
#include "jsoncpp/json/json.h"

struct memory {
  char *response;
  size_t size;
};
 
static size_t cb(void *data, size_t size, size_t nmemb, void *clientp)
{
  size_t realsize = size * nmemb;
  struct memory *mem = (struct memory *)clientp;
 
  void *ptr = realloc(reinterpret_cast<void*>(mem->response), mem->size + realsize + 1);
  if(ptr == NULL)
    return 0;  /* out of memory! */
 
  mem->response = reinterpret_cast<char*>(ptr);
  memcpy(&(mem->response[mem->size]), data, realsize);
  mem->size += realsize;
  mem->response[mem->size] = 0;
 
  return realsize;
}

inline std::string chat_gpt_call(CURL* curl, std::vector<std::string>& talks, const char* question)
{
    // 设置API访问参数
    const std::string url = "https://api.openai.com/v1/chat/completions";
    const std::string api_key = "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"; 

    // 设置请求头
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    headers = curl_slist_append(headers, ("Authorization: Bearer " + api_key).c_str());

    const char* ask_model = "{\"role\": \"user\", \"content\": \"%s\"}";
    const char* reply_model = "{\"role\": \"system\", \"content\": \"%s\"}";
    // 设置POST数据
    //std::string data = "{\"model\": \"gpt-3.5-turbo\","
    // "\"messages\": [{\"role\": \"system\", \"content\": \"You are a kind helpful assistant.\"}, "
    // "{\"role\": \"user\", \"content\": \"如果我有个公网ip就好了，这样别人就能访问我本地的服务了。\"}]"
    // "}";
    
    // +1 以容纳字符串结尾的'\0'否则我们将丢失最后一个字符
    size_t true_buff_len = snprintf(nullptr, 0, ask_model, question) + 1;
    //auto iask_buff = std::make_unique<char[]>(true_buff_len);
    auto iask_buff = std::unique_ptr<char[]>(new char[true_buff_len]);
    snprintf(iask_buff.get(), true_buff_len, ask_model, question);

    talks.emplace_back(iask_buff.get());

     const char* post_std = "{\"model\": \"gpt-3.5-turbo\","
     "%s]"
     "}";

    std::string q_a;
    for(auto& i : talks)
    {
        q_a += i;
        q_a += ',';
    }
    q_a.back() = '\0';

    //size_t body_len = strlen(post_std) - 2;
    //body_len += q_a.length();
    //auto data = std::make_unique<char[]>(body_len + 1);
    ////char* data = new char[body_len];
    ////data.get()[body_len] = '\0';
    //snprintf(data.get(), body_len, post_std, q_a.c_str());
    size_t body_len = snprintf(nullptr, 0, post_std, q_a.c_str());
    //auto data = std::make_unique<char[]>(body_len + 1 + 1);
    auto data = std::unique_ptr<char[]>(new char[body_len + 1 + 1]);

    snprintf(data.get(), body_len + 1, post_std, q_a.c_str());
    data.get()[body_len + 1] = '\0';

    //std::cout << data.get() << std::endl;

    //std::cout << data << std::endl;
    
    std::string response;

    if (curl) {
        
        // 设置URL
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        //curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);

        //设置代理端口
        curl_easy_setopt(curl, CURLOPT_PROXY, "http://192.168.31.193:808");

        // 设置请求头
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        struct memory chunk = {0};

        /* send all data to this function  */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, cb);
        /* we pass our 'chunk' struct to the callback function */
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        // 设置POST请求和数据
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data.get());

        // 设置超时时间
        curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 20L); // 连接超时5秒
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, 50L);       // 请求超时10秒

        // 执行请求
        CURLcode res = curl_easy_perform(curl);

        // 处理响应
        if (res == CURLE_OK) {
            long http_code = 0;
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
            if (http_code == 200) {
                char* content_type_ptr = nullptr;
                // 获取Content-Type的值
                curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &content_type_ptr);
                response = chunk.response;
                Json::Value iv;
                Json::CharReaderBuilder rbuilder;
                auto ir = rbuilder.newCharReader();
                std::string error_code;
                ir->parse(&response.front(), &response.back(), &iv, &error_code);
                auto key_res = iv["choices"][0]["message"];
                response = key_res["content"].asCString();

                auto ireplylen = snprintf(nullptr, 0, reply_model, response.c_str()); 
                ireplylen++;
                auto irp = std::unique_ptr<char[]>(new char[ireplylen]);
                snprintf(irp.get(), ireplylen, reply_model, response.c_str()); 

                //Json::StreamWriterBuilder isb;
                //isb.settings_["indentation"] = "";
                //isb.settings_["emitUTF8"] = true;
                //isb.settings_["commentStyle"] = "None";
                //auto str2 = Json::writeString(isb, key_res);
                
                talks.emplace_back(irp.get());
                //std::cout << "Response: " << response << std::endl;
            } else {
                std::cerr << "HTTP error: " << http_code << std::endl;
                response = "http request error!";
            }
        } else {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
            response = "http connect error!";
        }
        free(chunk.response);
    }

    return response; 
}

class chatGPT
{
public:
    chatGPT(const char *usr)
    {
        _usr = usr;
        _curl = curl_easy_init();
        _talks.emplace_back("\"messages\": [{\"role\": \"system\", \"content\": \"You are a kind helpful assistant.\"}");
    };
    ~chatGPT()
    {
        // 释放资源
        curl_easy_cleanup(_curl);
    }

    std::string ask(const char* a)
    {
        return chat_gpt_call(_curl, _talks, a);
    }

    void reset()
    {
        _talks.clear();
        _talks.emplace_back("\"messages\": [{\"role\": \"system\", \"content\": \"You are a kind helpful assistant.\"}");
    }

private:
    //std::string chat_gpt_call(CURL *curl, std::vector<std::string> &talks,
    // const char *question);

private:
    CURL *_curl = nullptr;
    std::vector<std::string> _talks;
    std::string _usr;
};