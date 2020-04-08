import org.openqa.selenium.JavascriptExecutor;
import org.openqa.selenium.Keys;
import org.openqa.selenium.WebDriver;
import org.openqa.selenium.firefox.FirefoxOptions;
import org.openqa.selenium.remote.DesiredCapabilities;
import org.openqa.selenium.remote.RemoteWebDriver;
import java.net.MalformedURLException;
import java.net.URL;

public class TestJni {

    private static final String HUB_URL = "http://localhost:4444/wd/hub";

    public static String getResult(String url){
        StringBuffer writer = new StringBuffer();
        try {
            WebDriver driver = getFirefoxDriver();
            writer.append("driver was created\n");
            driver.get(url);
            writer.append("Requested ");
            writer.append(url);
            writer.append("page was opened");
            driver.quit();
        } catch (Throwable e) {
            writer.append("Error occurred ");
            e.printStackTrace();
        }
        return writer.toString();
    }

    private static WebDriver getFirefoxDriver() throws MalformedURLException {
        FirefoxOptions options = new FirefoxOptions();
        WebDriver driver = null;
        return new RemoteWebDriver(new URL(HUB_URL), options);
    }

}
