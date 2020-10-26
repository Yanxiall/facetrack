package com.HYX.WebsocketClient.service.Impl;

import com.HYX.WebsocketClient.dao.photosMapper;
import com.HYX.WebsocketClient.dao.userMapper;
import com.HYX.WebsocketClient.entity.photos;
import com.HYX.WebsocketClient.service.DetectResultService;
import com.HYX.WebsocketClient.util.PageResult;
import com.HYX.WebsocketClient.util.PageUtil;
import org.apache.ibatis.annotations.Mapper;
import org.springframework.stereotype.Service;

import javax.annotation.Resource;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

@Service
public class DetectResultServiceImpl implements DetectResultService {
    @Resource
    private photosMapper PhotosMapper;
    @Override
    public PageResult getImgForIndexPage(int page){
        Map params = new HashMap();
        params.put("page", page);
        //4 record of every page
        params.put("limit", 8);
        PageUtil pageUtil = new PageUtil(params);

        List<photos> photosList = PhotosMapper.findImg(pageUtil);
        int total = PhotosMapper.getTotalImg(pageUtil);

        PageResult pageResult = new PageResult(photosList, total, pageUtil.getLimit(), pageUtil.getPage());
        return pageResult;
    }
}
