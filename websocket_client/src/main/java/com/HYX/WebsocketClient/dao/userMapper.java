package com.HYX.WebsocketClient.dao;

import com.HYX.WebsocketClient.entity.user;
import org.apache.ibatis.annotations.Param;
import org.springframework.stereotype.Component;

@Component
public interface userMapper {
    int deleteByPrimaryKey(Integer adminUserId);

    int insert(user record);

    int insertSelective(user record);

    user selectByPrimaryKey(Integer adminUserId);

    user search(@Param("username") String username, @Param("password") String password);

    int updateByPrimaryKeySelective(user record);

    int updateByPrimaryKey(user record);
}