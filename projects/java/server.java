import java.io.File;
import java.io.IOException;
import java.net.InetSocketAddress;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.concurrent.Executors;
import java.util.concurrent.ThreadPoolExecutor;

import com.sun.net.httpserver.HttpExchange;
import com.sun.net.httpserver.HttpHandler;
import com.sun.net.httpserver.HttpServer;

class Server {
    public static void main(String[] args) {
        try {
            var server = HttpServer.create(new InetSocketAddress("0.0.0.0", 8080), 0);
            server.createContext("/", new HelloWorldHttpHandler());
            var threadPoolExecutor = (ThreadPoolExecutor) Executors.newFixedThreadPool(10);
            server.setExecutor(threadPoolExecutor);
            server.start();
            System.out.println("Server started on port 8080");
        } catch (IOException exception) {
            return;
        }
    }
}

class HelloWorldHttpHandler implements HttpHandler {
    @Override
    public void handle(HttpExchange exchange) throws IOException {
        if (!exchange.getRequestMethod().equals("GET")) {
            exchange.sendResponseHeaders(405, 0);
            return;
        }
        var out = exchange.getResponseBody();
        var data = Files.readAllBytes(Paths.get("hello.html"));
        exchange.sendResponseHeaders(200, data.length);
        out.write(data);
        out.flush();
        out.close();
    }
}
