HTTPClient http;
    //http.begin(WiFiClient, urlFinal.c_str());
    {WiFiClient client;
            http.begin(client, urlFinal.c_str());
    }
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);//
    int httpCode = http.GET(); 
    //Serial.print("HTTP Status Code: ");
    //Serial.println(httpCode);
     
    //---------------------------------------------------------------------
    //getting response from google sheet
    String payload;
    if (httpCode > 0) {
        payload = http.getString();
        Serial.println("Payload: "+payload);  
    }
  http.end();
