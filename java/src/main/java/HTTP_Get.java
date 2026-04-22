import com.google.gson.Gson;
import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import java.net.http.WebSocket;

class TokenResponse {
  String token;
}

public class HTTP_Get {
  class ChatListener implements WebSocket.Listener {
    @Override
    public CompletionStage<?> onText(WebSocket webSocket, CharSequence data, boolean last) {
      // code goes here
      ;
      // this tells java to keep listening for the next message
      return WebSocket.Listener.super.onText(webSocket, data, last);
    }
  }

  void get_request() {
    try {
      HttpRequest request =
          HttpRequest.newBuilder()
              .GET()
              .uri(
                  URI.create(
                      "https://hackattic.com/challenges/websocket_chit_chat/"
                          + "problem?access_token=840aa5334d071243"))
              .build();

      HttpResponse<String> response =
          HttpClient.newHttpClient().send(request, HttpResponse.BodyHandlers.ofString());

      Gson gson = new Gson();
      TokenResponse parsedData = gson.fromJson(response.body(), TokenResponse.class);
      System.out.println("Parsed token: " + parsedData.token);
    } catch (Exception e) {
      System.out.println(e);
    }
  }

  public static void main(String[] args) {
    HTTP_Get obj_get_request = new HTTP_Get();
    obj_get_request.get_request();
  }
}
