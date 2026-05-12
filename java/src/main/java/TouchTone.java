import com.google.gson.Gson;
import java.net.URI;
import java.net.http.HttpClient;
import java.net.http.HttpRequest;
import java.net.http.HttpResponse;
import java.util.Map;

class TouchTone {
  Gson gson = new Gson();

  String GetRequest() {
    try {
      HttpRequest request =
          HttpRequest.newBuilder()
              .GET()
              .uri(
                  URI.create(
                      "https://hackattic.com/challenges/touch_tone_dialing/"
                          + "problem?access_token=840aa5334d071243"))
              .build();

      HttpResponse<String> response =
          HttpClient.newHttpClient().send(request, HttpResponse.BodyHandlers.ofString());

      System.out.println("Response body: " + response.body());
      Map<String, String> parsed = gson.fromJson(response.body(), Map.class);
      String wavUrl = parsed.get("wav_url");
      return wavUrl;

    } catch (Exception e) {
      System.out.println(e);
      return null;
    }
  }

  public static void main(String[] args) {
    TouchTone touchtone = new TouchTone();
    String wavUrl = touchtone.GetRequest();

    if (wavUrl != null) {
      ;
    } else {
      System.out.println("Error occured");
    }
  }
}
