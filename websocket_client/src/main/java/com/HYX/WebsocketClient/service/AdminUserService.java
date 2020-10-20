package com.HYX.WebsocketClient.service;

import com.HYX.WebsocketClient.entity.user;


public interface AdminUserService {

     user login(String username, String password);
     user getUserDetailById(Integer loginUserId);
     Boolean updatePassword(Integer loginUserId, String originalPassword, String newPassword);
     Boolean updateName(Integer loginUserId, String loginUserName, String nickName);

}
