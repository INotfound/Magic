#include "gtest/gtest.h"
#include "Magic/Utilty/String.h"

TEST(MagicStringTest,StringAsInt){
    EXPECT_EQ(Magic::StringAs<int8_t>("100",10),100);
    EXPECT_EQ(Magic::StringAs<int16_t>("100",10),100);
    EXPECT_EQ(Magic::StringAs<int32_t>("100",10),100);
    EXPECT_EQ(Magic::StringAs<int64_t>("100",10),100);

    EXPECT_NE(Magic::StringAs<int8_t>("10000",10),100);

    EXPECT_GT(Magic::StringAs<int8_t>("100.1",10),99);
    EXPECT_LT(Magic::StringAs<int8_t>("100.1",10),101);
    EXPECT_EQ(Magic::StringAs<int8_t>("100.1",10),100);

    EXPECT_GT(Magic::StringAs<int16_t>("100.1",10),99);
    EXPECT_LT(Magic::StringAs<int16_t>("100.1",10),101);
    EXPECT_EQ(Magic::StringAs<int16_t>("100.1",10),100);


    EXPECT_GT(Magic::StringAs<int32_t>("100.1",10),99);
    EXPECT_LT(Magic::StringAs<int32_t>("100.1",10),101);
    EXPECT_EQ(Magic::StringAs<int32_t>("100.1",10),100);


    EXPECT_GT(Magic::StringAs<int64_t>("100.1",10),99);
    EXPECT_LT(Magic::StringAs<int64_t>("100.1",10),101);
    EXPECT_EQ(Magic::StringAs<int64_t>("100.1",10),100);


}

TEST(MagicStringTest,StringAsDoubleAndFloat){
    EXPECT_EQ(100.1f,Magic::StringAs<float>("100.1"));
    /// double Not Use xxx.xf
    EXPECT_EQ(100.12333,Magic::StringAs<double>("100.12333"));
}

TEST(MagicStringTest,NumberAsString){
    EXPECT_STREQ(Magic::AsString(20).c_str(),"20");
    EXPECT_STREQ(Magic::AsString(20.f).c_str(),"20");
    EXPECT_STREQ(Magic::AsString(20.1).c_str(),"20.1");
    EXPECT_STREQ(Magic::AsString(20.1f).c_str(),"20.1");

    EXPECT_STRNE(Magic::AsString(20.0).c_str(),"20.0");
    EXPECT_STRNE(Magic::AsString(20.f).c_str(),"20.0");
}

TEST(MagicStringTest,StringToLowerAndToUpper){
    std::string lowerString = "asdfghjkl";
    std::string upperString = "ASDFGHJKL";
    EXPECT_STREQ(Magic::ToLower(upperString).c_str(),lowerString.c_str());
    EXPECT_STREQ(Magic::ToUpper(lowerString).c_str(),upperString.c_str());

    std::string  randomString = "aSdFgHJkL";
    EXPECT_STREQ(Magic::ToLower(randomString).c_str(),lowerString.c_str());
    EXPECT_STREQ(Magic::ToUpper(randomString).c_str(),upperString.c_str());
}

TEST(MagicStringTest,TrimString){
    std::string spaceString = " Hello World! ";
    EXPECT_STREQ(Magic::Trim(spaceString).c_str(),"Hello World!");
    std::string symbolString = "+Hello World!+";
    EXPECT_STRNE(Magic::Trim(symbolString).c_str(),"Hello World!");
    EXPECT_STREQ(Magic::Trim(symbolString,"+").c_str(),"Hello World!");
}

TEST(MagicStringTest,SubString){
    std::string spaceString = "Hello World!";
    EXPECT_STREQ(Magic::SubString(spaceString,0," ").c_str(),"Hello");
}

TEST(MagicStringTest,SplitString){
    std::string spaceString = "Hello World INotFound!";
    auto vec = Magic::Split(spaceString," ");
    EXPECT_STREQ(vec.at(0).c_str(),"Hello");
    EXPECT_STREQ(vec.at(1).c_str(),"World");
    EXPECT_STREQ(vec.at(2).c_str(),"INotFound!");
}

TEST(MagicStringTest,TimeString){
    std::time_t time = 1673686220;
    EXPECT_STREQ(Magic::TimeToString(time).c_str(),"2023-01-14 16:50:20");
}

TEST(MagicStringTest,ReplaceString){
    std::string spaceString = "Hello World INotFound!";
    EXPECT_STREQ(Magic::Replace(spaceString," World","").c_str(),"Hello INotFound!");
    EXPECT_STREQ(Magic::Replace(spaceString,"Hello World","Welcome").c_str(),"Welcome INotFound!");
}