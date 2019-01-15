#include <stdio.h>
#include <string.h>
 
#include <curl/curl.h>
 
int main(int argc, char *argv[])
{
  CURL *curl;
  CURLcode res;
 
  int ret;
  //char *url;

  struct curl_httppost *formpost=NULL;
  struct curl_httppost *lastptr=NULL;
  struct curl_slist *headerlist=NULL;
  static const char buf[] = "Expect:";
 
  //sprintf(url, "http://sup-heal.com:9080/picture");

  curl_global_init(CURL_GLOBAL_ALL);

  curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "filename",
               CURLFORM_COPYCONTENTS, argv[1]);
  
  curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "picture",
               CURLFORM_FILE, argv[1],
               CURLFORM_CONTENTTYPE, "image/jpeg");
              
  curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "filename1",
               CURLFORM_COPYCONTENTS, argv[2]);
  
  curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "picture1",
               CURLFORM_FILE, argv[2],
               CURLFORM_CONTENTTYPE, "image/jpeg");

  curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "filename2",
               CURLFORM_COPYCONTENTS, argv[3]);
  
  curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "picture2",
               CURLFORM_FILE, argv[3],
               CURLFORM_CONTENTTYPE, "image/jpeg");
            
  curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "filename3",
               CURLFORM_COPYCONTENTS, argv[4]);
  
  curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "picture3",
               CURLFORM_FILE, argv[4],
               CURLFORM_CONTENTTYPE, "image/jpeg");
  
  curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "filename4",
               CURLFORM_COPYCONTENTS, argv[5],
               CURLFORM_END);
  
  curl_formadd(&formpost,
               &lastptr,
               CURLFORM_COPYNAME, "picture5",
               CURLFORM_FILE, argv[5],
               CURLFORM_CONTENTTYPE, "image/jpeg",
               CURLFORM_END);
 
  curl = curl_easy_init();
  /* initalize custom header list (stating that Expect: 100-continue is not
     wanted */ 
  headerlist = curl_slist_append(headerlist, buf);
  if(curl) {
    /* what URL that receives this POST */ 
    curl_easy_setopt(curl, CURLOPT_URL, "http://www.sup-heal.com:9080/picture/FiveimageUpload");
    if ( (argc == 3) && (!strcmp(argv[2], "noexpectheader")) )
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
