package com.covidselfcare.schedular;

public class Model_lecture {
    int id;
    String name,discription,createdtime;

    public Model_lecture(int id, String name, String discription, String createdtime) {
        this.id = id;
        this.name = name;
        this.discription = discription;
        this.createdtime = createdtime;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getDiscription() {
        return discription;
    }

    public void setDiscription(String discription) {
        this.discription = discription;
    }

    public String getCreatedtime() {
        return "Created on: "+createdtime;
    }

    public void setCreatedtime(String createdtime) {
        this.createdtime = createdtime;
    }
}
