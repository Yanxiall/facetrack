package com.HYX.WebsocketClient.dao;

import com.HYX.WebsocketClient.entity.photos;
import org.springframework.stereotype.Component;

import java.util.List;
import java.util.Map;
@Component
public interface photosMapper {
    int deleteByPrimaryKey(Long imgId);

    int insert(photos record);

    int insertSelective(photos record);

    photos selectByPrimaryKey(Long imgId);

    int updateByPrimaryKeySelective(photos record);

    int updateByPrimaryKey(photos record);

    List<photos> findImg(Map param);

    int getTotalImg(Map param);
}