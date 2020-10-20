package com.HYX.WebsocketClient.controller.config;

import com.google.code.kaptcha.impl.DefaultKaptcha;
import org.springframework.context.annotation.Bean;
import org.springframework.stereotype.Component;
import com.google.code.kaptcha.util.Config;
import java.util.Properties;

@Component
public class KaptchaConfig {
    @Bean
    public DefaultKaptcha getDefaultKaptcha(){
        com.google.code.kaptcha.impl.DefaultKaptcha defaultKaptcha = new com.google.code.kaptcha.impl.DefaultKaptcha();
        Properties properties = new Properties();
        // image framework
        properties.put("kaptcha.border", "no");
        // captcha color
        properties.put("kaptcha.textproducer.font.color", "black");
        // image width
        properties.put("kaptcha.image.width", "160");
        // image length
        properties.put("kaptcha.image.height", "40");
        // font size
        properties.put("kaptcha.textproducer.font.size", "30");
        // captcha length
        properties.put("kaptcha.textproducer.char.space", "5");
        // font family
        properties.setProperty("kaptcha.textproducer.font.names", "宋体,楷体,微软雅黑");
        Config config = new Config(properties);
        defaultKaptcha.setConfig(config);
        return defaultKaptcha;
    }
}
