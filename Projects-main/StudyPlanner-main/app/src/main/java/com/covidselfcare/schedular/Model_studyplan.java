package com.covidselfcare.schedular;

public class Model_studyplan {
    int id;
    String name,myplan,datetime;
    public Model_studyplan(int id,String name, String myplan, String datetime) {
        this.name = name;
        this.myplan = myplan;
        this.datetime=datetime;
        this.id=id;
    }

    public int getId(){
        return id;
    }
    public void setId(int id){
        this.id=id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }
    public void setDatetime(String datetime){
        this.datetime=datetime;
    }

    public String getMyplan() {
        return myplan;
    }

    public void setMyplan(String myplan) {
        this.myplan = myplan;
    }

    public String getDatetime(){
        return "Created on: "+datetime;
    }
}
