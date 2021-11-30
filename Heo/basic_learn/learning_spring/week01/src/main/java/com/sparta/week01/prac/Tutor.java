package com.sparta.week01.prac;

public class Tutor {
    private String name;
    private String bio;

    public Tutor() {

    }

    public Tutor(String name, String bio) {
        this.name = name;
        this.bio = bio;
    }

    public void setName(String name) {
        this.name = name;
    }
    public void setBio(String bio) {
        this.bio = bio;
    }

    public String getName() {
        return this.name;
    }

    public String getBio() {
        return this.bio;
    }
}
