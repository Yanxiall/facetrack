package com.HYX.WebsocketClient.service.Impl;

import com.HYX.WebsocketClient.dao.userMapper;
import com.HYX.WebsocketClient.entity.user;
import com.HYX.WebsocketClient.service.AdminUserService;
import com.HYX.WebsocketClient.util.MD5Util;
import org.springframework.stereotype.Service;
import javax.annotation.Resource;


@Service
public class AdminUserServiceImpl implements AdminUserService {
    @Resource
    private userMapper Usermapper;

    @Override
    public user login(String username,String password){
        String EncryPassword = MD5Util.MD5Encode(password, "UTF-8");
        return Usermapper.search(username,EncryPassword);
    }
    @Override
    public user getUserDetailById(Integer loginUserId) {
        return Usermapper.selectByPrimaryKey(loginUserId);
    }
    @Override
    public Boolean updatePassword(Integer loginUserId, String originalPassword, String newPassword) {
        user adminUser = Usermapper.selectByPrimaryKey(loginUserId);
        if (adminUser != null) {
            String originalPasswordMd5 = MD5Util.MD5Encode(originalPassword, "UTF-8");
            String newPasswordMd5 = MD5Util.MD5Encode(newPassword, "UTF-8");
            //change password only when the original password is correct
            if (originalPasswordMd5.equals(adminUser.getLoginPassword())) {
                //set new password
                adminUser.setLoginPassword(newPasswordMd5);
                if (Usermapper.updateByPrimaryKeySelective(adminUser) > 0) {
                    //return true when succeed
                    return true;
                }
            }
        }
        return false;
    }

    @Override
    public Boolean updateName(Integer loginUserId, String loginUserName, String nickName) {
        user adminUser = Usermapper.selectByPrimaryKey(loginUserId);
        if (adminUser != null) {
            //set new login UserName and nickName
            adminUser.setLoginUserName(loginUserName);
            adminUser.setNickName(nickName);
            if (Usermapper.updateByPrimaryKeySelective(adminUser) > 0) {
                //return true when succeed
                return true;
            }
        }
        return false;
    }


}

