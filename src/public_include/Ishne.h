//
//  Ishne.h
//  Spaulding Medical Test
//
//  Created by James Folk on 11/6/20.
//  Copyright © 2020 NJLIGames Ltd. All rights reserved.
//

#include <string>
#include <map>

class Ishne {
public:
    using LeadVector = std::vector<short>;
//    using SampleVector = std::vector<LeadVector>;
    using Map = std::map<size_t, LeadVector*>;
    using Pair = std::pair<size_t, LeadVector*>;
private:
    struct ISNEHeader{
        long Var_length_block_size;
        long Sample_Size_ECG;
        long Offset_var_lenght_block;
        long Offset_ECG_block;
        short int File_Version;
        unsigned char First_Name[40];
        unsigned char Last_Name[40];
        unsigned char ID[20];
        short int Sex;
        short int Race;
        short int Birth_Date[3];
        short int Record_Date[3];
        short int File_Date[3];
        short int Start_Time[3];
        short int nLeads;
        short int Lead_Spec[12];
        short int Lead_Qual[12];
        short int Resolution[12];
        short int Pacemaker;
        unsigned char Recorder[40];
        short int Sampling_Rate;
        unsigned char Proprietary[80];
        unsigned char Copyright[80];
        unsigned char Reserved[88];
    };
    ISNEHeader *mData;
    void *mFileData;
    long mFileSize;
    
    Map *mMap;
    char *mVarblock;
    short mMax;
    short mMin;
public:
    
    
    Ishne();
    ~Ishne();
    
    bool init(const std::string &fileName);
    void unInit();
    
    Ishne(const Ishne &rhs) = delete;
    Ishne(const Ishne &&rhs) = delete;
    const Ishne& operator=(const Ishne &rhs) = delete;
    const Ishne&& operator=(const Ishne &&rhs) = delete;
    
    short numberOfLeads()const;
    short samplingRate()const;
    
    const Ishne::LeadVector &getValues(const size_t lead_index)const;
    const short &maxValue()const{return mMax;}
    const short &minValue()const{return mMin;}
protected:
    void *load(const std::string &fileName);
    void unload();
};