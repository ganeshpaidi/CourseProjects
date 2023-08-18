package com.covidselfcare.schedular;

public class Model_assignment {
    int id;
    String name, discription,createdtime;
    int duetime;

    public Model_assignment(int id, String name, String discription, int duetime,String createdtime) {
        this.id = id;
        this.name = name;
        this.discription = discription;
        this.createdtime = createdtime;
        this.duetime = duetime;
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
        return createdtime;
    }

    public void setCreatedtime(String createdtime) {
        this.createdtime = createdtime;
    }

    public String getDuetime() {

        if(duetime==0){
            return "Not valid date entry";
        }
        return "Due date: "+String.valueOf(duetime);
    }

    public void setDuetime(int duetime) {
        this.duetime = duetime;
    }
}

