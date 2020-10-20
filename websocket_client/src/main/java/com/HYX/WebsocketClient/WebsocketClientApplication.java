package com.HYX.WebsocketClient;

import org.mybatis.spring.annotation.MapperScan;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

@SpringBootApplication
@MapperScan("com.HYX.WebsocketClient.dao")
public class WebsocketClientApplication {

	public static void main(String[] args) {
		SpringApplication.run(WebsocketClientApplication.class, args);
	}

}
