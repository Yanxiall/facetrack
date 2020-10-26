package com.HYX.WebsocketClient.util;

import java.io.Serializable;
import java.util.List;

public class PageResult implements Serializable {

    //total records
    private int totalCount;
    //record number of every page
    private int pageSize;
    //total pages
    private int totalPage;
    //current page
    private int currPage;
    public List<?> list;


    public PageResult(List<?> list, int totalCount, int pageSize, int currPage) {
        this.list = list;//the data list of query
        this.totalCount = totalCount;//the total records
        this.pageSize = pageSize;//the number of records of every page
        this.currPage = currPage;//the query page
        this.totalPage = (int)Math.ceil((double)totalCount/pageSize);//the total pages
    }

    public int getTotalCount() {
        return totalCount;
    }

    public void setTotalCount(int totalCount) {
        this.totalCount = totalCount;
    }

    public int getPageSize() {
        return pageSize;
    }

    public void setPageSize(int pageSize) {
        this.pageSize = pageSize;
    }

    public int getTotalPage() {
        return totalPage;
    }

    public void setTotalPage(int totalPage) {
        this.totalPage = totalPage;
    }

    public int getCurrPage() {
        return currPage;
    }

    public void setCurrPage(int currPage) {
        this.currPage = currPage;
    }

    public List<?> getList() {
        return list;
    }

    public void setList(List<?> list) {
        this.list = list;
    }

}
