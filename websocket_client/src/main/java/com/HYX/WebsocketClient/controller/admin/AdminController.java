package com.HYX.WebsocketClient.controller.admin;

import com.HYX.WebsocketClient.entity.user;
import com.HYX.WebsocketClient.service.AdminUserService;
import com.HYX.WebsocketClient.service.DetectResultService;
import com.HYX.WebsocketClient.util.PageResult;
import org.springframework.stereotype.Controller;
import org.springframework.util.ResourceUtils;
import org.springframework.web.bind.annotation.*;
import org.springframework.web.multipart.MultipartFile;

import javax.annotation.Resource;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;
import java.io.File;
import java.io.IOException;
import java.net.URI;
import java.net.URISyntaxException;


@Controller
@RequestMapping("/admin")

public class AdminController {

    @Resource
    private AdminUserService adminUserService;
    @Resource
    private DetectResultService detectResultService;

    @GetMapping({"/login"})
    public String login() {
        return "admin/login";
    }
    @GetMapping({"/client"})
    public String client() {
        return "admin/client";
    }


    @PostMapping(value = "/login")
    public String login(@RequestParam("userName") String userName,
                        @RequestParam("password") String password,
                        @RequestParam("VerifyCode") String VerifyCode,
                        HttpSession session) {
        //verify the VerifyCode
        String captcha = session.getAttribute("verifyCode") + "";
        if(!VerifyCode.equals(captcha)){
            session.setAttribute("errorMsg", "VerifyCode is wrong");
            return "admin/login";
        }
        //search the adminUser to see if it exists
        user User = adminUserService.login(userName, password);
        if (User != null) {
            session.setAttribute("loginUser", User.getNickName());
            session.setAttribute("loginUserId", User.getAdminUserId());
            // session will be open for 2 hours
            // session.setMaxInactiveInterval(60 * 60 * 2);
            return "redirect:/admin/index";
        } else {
            session.setAttribute("errorMsg", "userName or password is wrong");
            return "admin/login";
        }
    }
    @GetMapping({"", "/", "/index", "/index.html"})
    public String index() {
        return "admin/index";
    }

    @GetMapping("/profile")
    public String profile(HttpServletRequest request) {
        Integer loginUserId = (int) request.getSession().getAttribute("loginUserId");
        user User = adminUserService.getUserDetailById(loginUserId);
        if (User == null) {
            return "admin/login";
        }
        request.setAttribute("path", "profile");
        request.setAttribute("loginUserName", User.getLoginUserName());
        request.setAttribute("nickName", User.getNickName());
        return "admin/profile";
    }
    @PostMapping("/profile/password")
    @ResponseBody
    public String passwordUpdate(HttpServletRequest request, @RequestParam("originalPassword") String originalPassword,
                                 @RequestParam("newPassword") String newPassword) {


        Integer loginUserId = (int) request.getSession().getAttribute("loginUserId");
        if (adminUserService.updatePassword(loginUserId, originalPassword, newPassword)) {
            //clear the data in session
            request.getSession().removeAttribute("loginUserId");
            request.getSession().removeAttribute("loginUser");
            request.getSession().removeAttribute("errorMsg");
            return "success";
        } else {
            return "fail";
        }
    }
    @PostMapping("/profile/name")
    @ResponseBody
    public String nameUpdate(HttpServletRequest request, @RequestParam("UserName") String UserName,
                             @RequestParam("NickName") String NickName) {

        Integer loginUserId = (int) request.getSession().getAttribute("loginUserId");
        if (adminUserService.updateName(loginUserId, UserName, NickName)) {
            return "success";
        } else {
            return "fail";
        }
    }


    @GetMapping("/logout")
    public String logout(HttpSession session) {
        session.removeAttribute("loginUser");
        session.removeAttribute("loginUserId");
        session.removeAttribute("errorMsg");
        return "admin/login";
    }
    //detected result webpage
    @GetMapping({"/result"})
    public String result(HttpServletRequest request) {
        return this.page(request, 1);
    }

    @GetMapping({"/page/{pageNum}"})
    @ResponseBody
    public String page(HttpServletRequest request, @PathVariable("pageNum") int pageNum) {
        PageResult DetectPageResult = detectResultService.getImgForIndexPage(pageNum);
        if (DetectPageResult == null) {
            return "error";
        }
        request.setAttribute("DetectPageResult", DetectPageResult);
        return "admin/result";
    }

}


