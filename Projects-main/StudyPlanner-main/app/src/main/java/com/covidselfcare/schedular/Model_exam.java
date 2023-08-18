package com.covidselfcare.schedular;

public class Model_exam {
    int id;
    String name,discription;
    int quiztime;
    String createdtime;

    public Model_exam(int id, String name, String discription, int quiztime, String createdtime) {
        this.id = id;
        this.name = name;
        this.discription = discription;
        this.quiztime = quiztime;
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

    public String getQuiztime() {

        if(quiztime==0){
            return "Not valid date Entry";
        }

        return "Quiz date: "+String.valueOf(quiztime);
    }

    public void setQuiztime(int quiztime) {
        this.quiztime = quiztime;
    }

    public String getCreatedtime() {
        return createdtime;
    }

    public void setCreatedtime(String createdtime) {
        this.createdtime = createdtime;
    }
}

