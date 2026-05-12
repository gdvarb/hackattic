import com.google.gson.Gson;
import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import java.net.http.WebSocket;
import java.time.Duration;
import java.time.Instant;
import java.util.Scanner;
import java.util.concurrent.CompletionStage;

class TokenResponse {
  String token;
}

class ChatListener implements WebSocket.Listener {
  Instant previousTime;
  String secretKey;

  public String getSecret() {
    return secretKey;
  }

  @Override
  public void onOpen(WebSocket webSocket) {
    previousTime = Instant.now();
    WebSocket.Listener.super.onOpen(webSocket);
  }

  @Override
  public CompletionStage<?> onText(WebSocket webSocket, CharSequence data, boolean last) {
    // code goes here
    String differenceString;
    System.out.println("Server output: " + data);

    if (data.toString().equals("good!") || data.toString().startsWith("hello!")) {
      ;
    } else if (data.toString().equals("ping!")) {
      int[] intervals = {700, 1500, 2000, 2500, 3000};
      double minDifference = Double.POSITIVE_INFINITY;
      int normalizedDifference = 0;

      Instant currentTime = Instant.now();
      long difference = Duration.between(previousTime, currentTime).toMillis();
      previousTime = currentTime;

      for (int i = 0; i < intervals.length; i++) {
        long calculatedDifference = Math.abs(difference - intervals[i]);
        if (calculatedDifference < minDifference) {
          minDifference = calculatedDifference;
          normalizedDifference = intervals[i];
        }
      }

      differenceString = String.valueOf(normalizedDifference);
      // send text back
      webSocket.sendText(differenceString, true);

    } else {
      // secrete key logic
      // secretKey = data.toString();
      System.out.println("Secrete key obtained: " + data);
      int secretKeyStart = data.toString().indexOf("\"");
      secretKey = data.toString().substring(secretKeyStart + 1, data.toString().length() - 1);
      System.out.println("substring: " + secretKey);
    }

    // this tells java to keep listening for the next message
    return WebSocket.Listener.super.onText(webSocket, data, last);
  }
}

public class HTTP_Get {
  ChatListener chat_listener = new ChatListener();

  void connect_websocket(String wsToken) {
    try {
      // Create client (the phone)
      HttpClient client = HttpClient.newHttpClient();

      // Dial the number and attach Listener
      WebSocket ws =
          client
              .newWebSocketBuilder()
              .buildAsync(URI.create("wss://hackattic.com/_/ws/" + wsToken), chat_listener)
              .join(); // join() waits for connection to actually establish
      System.out.println("Websocket connected");
    } catch (Exception e) {
      System.out.println(e);
    }
  }

  String get_request() {
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
      return parsedData.token;
    } catch (Exception e) {
      System.out.println(e);
      return null;
    }
  }

  String post_request() {
    String secret_key = chat_listener.getSecret();
    String jsonPayloadFormat = "{\"secret\": \"%s\"}";
    String jsonPayload = String.format(jsonPayloadFormat, secret_key);
    try {
      HttpRequest request =
          HttpRequest.newBuilder()
              .POST(HttpRequest.BodyPublishers.ofString(jsonPayload))
              .uri(
                  URI.create(
                      "https://hackattic.com/challenges/websocket_chit_chat/"
                          + "solve?access_token=840aa5334d071243"))
              .header("Content-Type", "application/json")
              .build();

      HttpResponse<String> response =
          HttpClient.newHttpClient().send(request, HttpResponse.BodyHandlers.ofString());

      System.out.println("Parsed token: " + response.body());
      return response.body();

    } catch (Exception e) {
      System.out.println(e);
      return null;
    }
  }

  public static void main(String[] args) {
    HTTP_Get obj_get_request = new HTTP_Get();
    String wsToken = obj_get_request.get_request();

    if (wsToken != null) {
      obj_get_request.connect_websocket(wsToken);
      Instant start = Instant.now();
      Scanner scanner = new Scanner(System.in);
      System.out.println("Press enter to continue...");
      scanner.nextLine();
      String postRequest = obj_get_request.post_request();
      System.out.println(postRequest);

    } else {
      System.out.println("Error occured");
    }
  }
}
