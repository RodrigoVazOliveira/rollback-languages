#include <iostream>
#include <string>
#include <curl/curl.h>


size_t write_callback(void* contents, size_t size, size_t nmenb, void* userp) {
    size_t total_size = size * nmenb;
    ((std::string*)userp)->append((char*)contents, total_size);
    return total_size;
}

int main(int argc, char* argv[]) {
    CURL* curl;
    CURLcode res;
    std::string url = "https://viacep.com.br/ws/01001000/json/";
    std::string cep, response_buffer;

    response_buffer.clear();
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    std::cout << "Digite seu cep" << std::endl;
    std::cin >> cep;
    url.replace(25, 8, cep);

    if (curl) {    
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        std::cout << "Realizando a requisicao" << std::endl;
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&response_buffer);
        std::cout << "Realizando a requisicao" << std::endl;
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            std::cout << "ocorreu um erro na requisicao: " << curl_easy_strerror(res) << std::endl;
        }

        long http_code = 0;
        curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
        std::cout << "código de resposta HTTP: " << http_code << std::endl;
        std::cout << "Resposta: " << response_buffer << std::endl;
    }    
    curl_global_cleanup();
    
    return 0;
}
