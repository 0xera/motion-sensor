#include <ESP8266WiFi.h> //импорты для работы программы
#include <WiFiClient.h>
#include <WiFiServer.h>

String header = "HTTP/1.1 200 OK Content-Type: text/html\r\n";

String HTML = R"(
  <!DOCTYPE html>
  <html>
   <head>
    <meta name="viewport" content="width=device-width, initial-scale=1.0"/>
    <meta charset="utf-8">
    <style>
      body {font-size:100%;}
      #main {display: table; margin: auto;  padding: 0 10px 0 10px; }
      h2 {text-align:center; }
      p { text-align:center; }
      .class_move { display:inline-block; text-align:center;}
    </style>
    <title>Motion Detector</title>
   </head>
   <body>
     <div id="main">
       <h2>Motion Detector</h2>
       <div id="content">
         <p id="time">&nbsp;</p>
         <h4 class="class_move">Is there movement?</h4>
         <h3 id="move" class="class_move">No</h3>
       </div>
     </div>
      <script>
      function refreshDate()  // метод для обновления времени
       {
        var date = new Date(); // получить объект с текущей датой
        var h=date.getHours(); // получить часы, минуты, секунды из объекта
        var m=date.getMinutes();
        var s=date.getSeconds();
        if(h<10){h="0"+h;}
        if(m<10){m="0"+m;}       
        if(s<10){s="0"+s;}
        document.getElementById("time").innerHTML = h+":"+m+":"+s; // заменить текст в HTML по тегу на текущее время HH:mm:ss
       }
      
      function ajaxLoad() // метод для отправки запроса и обработки ответа
      {
        var ajaxRequest = new XMLHttpRequest(); // создание запроса
        ajaxRequest.onreadystatechange = function() // создания обработчика события
        {
          if(this.readyState == 4 && this.status==200)
          {
           document.getElementById("move").innerHTML = this.responseText; // замена текста в HTML по тегу на текст в ответе 
          }
        }
        ajaxRequest.open('GET',"getMove",true); // асинхронно отвравляет запрос на получение новой информации
        ajaxRequest.send(); // отправка запроса
      }
     setInterval(refreshDate, 1000 ); // повторять каждую секунду оба метода
     setInterval(ajaxLoad, 1000 );
    </script>
   </body>
  </html>)"; 
  
String ssid = "npocu";       //  идентификатор SSID Wi-Fi сети 
String pass = "sglk4786";    //  пароль Wi-Fi сети
 
WiFiServer server(80); // создание сервера
 
bool isMoving = false; // переменная, которая будет хранить информацию о наличии движения
String request = ""; // переменна, в которую запишется запрос для дальнейшей обработки 
void setup() 
{
    Serial.begin(115200);
    connectToWiFi(); // вызов метода для соединения с сетью Wi-Fi
} 


void connectToWiFi() // Подключение к Wi-Fi сети
{
    WiFi.begin(ssid, pass); // подключится
    while (!isWiFiConnected()) // соединение ожидается в цикле
    {
        delay(500);
    }
    server.begin();  // Старт сервера
}
 
bool isWiFiConnected() // метод, проверяющий наличие соединения
{
  return WiFi.status() == WL_CONNECTED;
}
void loop() 
{
    WiFiClient client = server.available(); // получить клиента
    if (!client)  {  return;  } // Проверка наличия клиента
    request = client.readStringUntil('\r'); // прочитать запрос
    if ( request.indexOf("getMove") > 0 ) // проверка наличия строки в запросе
    {
      if (((char)Serial.read()) == '1' && !isMoving)  // Получение и проверка данных о движении, полученных через Serial порт с датчика, который подключен к Arduino
      {
        isMoving = true;
        sendStateAJAX("Yes", client); // отправить клиенту информцию о том, что движение есть
      } 
      else if(((char)Serial.read()) == '0' && isMoving)
      {
        isMoving = false;
        sendStateAJAX("No", client); // отправить клиенту информцию о том, что движения нету
      }   
    } 
    else
    {
        sendState(client); // отправить страницу клиенту
    }
    delay(1000); // секундная задержка
}

void sendStateAJAX(String state, WiFiClient client) // Отправка информации о движении 
{
     client.println( state );  // отправка
}
void sendState(WiFiClient client) // отправка HTML страницы
{
    client.flush(); // очистить содержимое страницы
    client.print( header ); // отправка страницы
    client.print( HTML );    
}
