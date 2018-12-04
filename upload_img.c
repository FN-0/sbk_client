#include <stdio.h>
#include <string.h>
 
#include <curl/curl.h>
 
int main(int argc, char *argv[])
{
  CURL *curl;
  CURLcode res;
 
  int ret;

  struct curl_httppost *formpost=NULL;
  struct curl_httppost *lastptr=NULL;
  struct curl_slist *headerlist=NULL;
  static const char buf[] = "Expect:";
 
  curl_global_init(CURL_GLOBAL_ALL);

  curl_formadd(&formpost,  
               &lastptr,  
               CURLFORM_COPYNAME, "filename", 
               CURLFORM_COPYCONTENTS, "00:0c:29:1a:cc:0a20181120223321.jpg",  
               CURLFORM_END);  
  
  /* Fill in the submit field too, even if this is rarely needed */  
  curl_formadd(&formpost,  
               &lastptr,  
               CURLFORM_COPYNAME, "picture",  
               /* TODO: using argv[1] */
               CURLFORM_FILE, "00:0c:29:1a:cc:0a20181120223321.jpg",
               CURLFORM_CONTENTTYPE, "image/jpeg",  
               CURLFORM_END);  
 
  curl = curl_easy_init();
  /* initalize custom header list (stating that Expect: 100-continue is not
     wanted */ 
  headerlist = curl_slist_append(headerlist, buf);
  if(curl) {
    /* what URL that receives this POST */ 
    curl_easy_setopt(curl, CURLOPT_URL, "http://192.168.43.138:8080/imageUpload?sid=2");
    if ( (argc == 2) && (!strcmp(argv[1], "noexpectheader")) )
      /* only disable 100-continue header if explicitly requested */ 
      curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
    curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
    /* abort if slower than 8192 bytes/sec during 30 seconds */
    curl_easy_setopt(curl, CURLOPT_LOW_SPEED_TIME, 30L);
    curl_easy_setopt(curl, CURLOPT_LOW_SPEED_LIMIT, 8192L);
    /* Perform the request, res will get the return code */ 
    res = curl_easy_perform(curl);
    /* Check for errors */ 
    if(res != CURLE_OK) {
      fprintf(stderr, "curl_easy_perform() failed: %s\n",
              curl_easy_strerror(res));
      ret = 2;
    }
    /* always cleanup */ 
    curl_easy_cleanup(curl);
 
    /* then cleanup the formpost chain */ 
    curl_formfree(formpost);
    /* free slist */ 
    curl_slist_free_all (headerlist);
  }
  if(ret == 2)
    return 2;
  return 0;
}
