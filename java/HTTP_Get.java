import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;

public class HTTP_Get {
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

      System.out.println(response.statusCode());
      System.out.println(response.body());
    } catch (Exception e) {
      System.out.println(e);
    }
  }

  public static void main(String[] args) {
    HTTP_Get obj_get_request = new HTTP_Get();
    obj_get_request.get_request();
  }
}
