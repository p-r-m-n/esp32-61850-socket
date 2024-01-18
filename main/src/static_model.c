/*
 * static_model.c
 *
 * automatically generated from arquivoSCL_criado.icd
 */
#include "static_model.h"

static void initializeValues();

extern DataSet iedModelds_ANALOGRW_LLN0_analogrw_dataset;


extern DataSetEntry iedModelds_ANALOGRW_LLN0_analogrw_dataset_fcda0;

DataSetEntry iedModelds_ANALOGRW_LLN0_analogrw_dataset_fcda0 = {
  "ANALOGRW",
  false,
  "GGIO1$MX$AnOut1$mxVal", 
  -1,
  NULL,
  NULL,
  NULL
};

DataSet iedModelds_ANALOGRW_LLN0_analogrw_dataset = {
  "ANALOGRW",
  "LLN0$analogrw_dataset",
  1,
  &iedModelds_ANALOGRW_LLN0_analogrw_dataset_fcda0,
  NULL
};

LogicalDevice iedModel_ANALOGRW = {
    LogicalDeviceModelType,
    "ANALOGRW",
    (ModelNode*) &iedModel,
    NULL,
    (ModelNode*) &iedModel_ANALOGRW_LLN0
};

LogicalNode iedModel_ANALOGRW_LLN0 = {
    LogicalNodeModelType,
    "LLN0",
    (ModelNode*) &iedModel_ANALOGRW,
    (ModelNode*) &iedModel_ANALOGRW_GGIO1,
    (ModelNode*) &iedModel_ANALOGRW_LLN0_Mod,
};

DataObject iedModel_ANALOGRW_LLN0_Mod = {
    DataObjectModelType,
    "Mod",
    (ModelNode*) &iedModel_ANALOGRW_LLN0,
    NULL,
    (ModelNode*) &iedModel_ANALOGRW_LLN0_Mod_stVal,
    0
};

DataAttribute iedModel_ANALOGRW_LLN0_Mod_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_ANALOGRW_LLN0_Mod,
    (ModelNode*) &iedModel_ANALOGRW_LLN0_Mod_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_ANALOGRW_LLN0_Mod_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_ANALOGRW_LLN0_Mod,
    (ModelNode*) &iedModel_ANALOGRW_LLN0_Mod_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_ANALOGRW_LLN0_Mod_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_ANALOGRW_LLN0_Mod,
    (ModelNode*) &iedModel_ANALOGRW_LLN0_Mod_ctlModel,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_ANALOGRW_LLN0_Mod_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_ANALOGRW_LLN0_Mod,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

LogicalNode iedModel_ANALOGRW_GGIO1 = {
    LogicalNodeModelType,
    "GGIO1",
    (ModelNode*) &iedModel_ANALOGRW,
    NULL,
    (ModelNode*) &iedModel_ANALOGRW_GGIO1_Mod,
};

DataObject iedModel_ANALOGRW_GGIO1_Mod = {
    DataObjectModelType,
    "Mod",
    (ModelNode*) &iedModel_ANALOGRW_GGIO1,
    (ModelNode*) &iedModel_ANALOGRW_GGIO1_AnOut1,
    (ModelNode*) &iedModel_ANALOGRW_GGIO1_Mod_stVal,
    0
};

DataAttribute iedModel_ANALOGRW_GGIO1_Mod_stVal = {
    DataAttributeModelType,
    "stVal",
    (ModelNode*) &iedModel_ANALOGRW_GGIO1_Mod,
    (ModelNode*) &iedModel_ANALOGRW_GGIO1_Mod_q,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_ENUMERATED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_ANALOGRW_GGIO1_Mod_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_ANALOGRW_GGIO1_Mod,
    (ModelNode*) &iedModel_ANALOGRW_GGIO1_Mod_t,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_ANALOGRW_GGIO1_Mod_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_ANALOGRW_GGIO1_Mod,
    (ModelNode*) &iedModel_ANALOGRW_GGIO1_Mod_ctlModel,
    NULL,
    0,
    IEC61850_FC_ST,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_ANALOGRW_GGIO1_Mod_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_ANALOGRW_GGIO1_Mod,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataObject iedModel_ANALOGRW_GGIO1_AnOut1 = {
    DataObjectModelType,
    "AnOut1",
    (ModelNode*) &iedModel_ANALOGRW_GGIO1,
    NULL,
    (ModelNode*) &iedModel_ANALOGRW_GGIO1_AnOut1_mxVal,
    0
};

DataAttribute iedModel_ANALOGRW_GGIO1_AnOut1_mxVal = {
    DataAttributeModelType,
    "mxVal",
    (ModelNode*) &iedModel_ANALOGRW_GGIO1_AnOut1,
    (ModelNode*) &iedModel_ANALOGRW_GGIO1_AnOut1_q,
    (ModelNode*) &iedModel_ANALOGRW_GGIO1_AnOut1_mxVal_f,
    0,
    IEC61850_FC_MX,
    IEC61850_CONSTRUCTED,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_ANALOGRW_GGIO1_AnOut1_mxVal_f = {
    DataAttributeModelType,
    "f",
    (ModelNode*) &iedModel_ANALOGRW_GGIO1_AnOut1_mxVal,
    NULL,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_FLOAT32,
    0 + TRG_OPT_DATA_CHANGED,
    NULL,
    0};

DataAttribute iedModel_ANALOGRW_GGIO1_AnOut1_q = {
    DataAttributeModelType,
    "q",
    (ModelNode*) &iedModel_ANALOGRW_GGIO1_AnOut1,
    (ModelNode*) &iedModel_ANALOGRW_GGIO1_AnOut1_t,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_QUALITY,
    0 + TRG_OPT_QUALITY_CHANGED,
    NULL,
    0};

DataAttribute iedModel_ANALOGRW_GGIO1_AnOut1_t = {
    DataAttributeModelType,
    "t",
    (ModelNode*) &iedModel_ANALOGRW_GGIO1_AnOut1,
    (ModelNode*) &iedModel_ANALOGRW_GGIO1_AnOut1_Oper,
    NULL,
    0,
    IEC61850_FC_MX,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_ANALOGRW_GGIO1_AnOut1_Oper = {
    DataAttributeModelType,
    "Oper",
    (ModelNode*) &iedModel_ANALOGRW_GGIO1_AnOut1,
    (ModelNode*) &iedModel_ANALOGRW_GGIO1_AnOut1_ctlModel,
    (ModelNode*) &iedModel_ANALOGRW_GGIO1_AnOut1_Oper_ctlVal,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_ANALOGRW_GGIO1_AnOut1_Oper_ctlVal = {
    DataAttributeModelType,
    "ctlVal",
    (ModelNode*) &iedModel_ANALOGRW_GGIO1_AnOut1_Oper,
    (ModelNode*) &iedModel_ANALOGRW_GGIO1_AnOut1_Oper_origin,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_FLOAT32,
    0,
    NULL,
    0};

DataAttribute iedModel_ANALOGRW_GGIO1_AnOut1_Oper_origin = {
    DataAttributeModelType,
    "origin",
    (ModelNode*) &iedModel_ANALOGRW_GGIO1_AnOut1_Oper,
    (ModelNode*) &iedModel_ANALOGRW_GGIO1_AnOut1_Oper_ctlNum,
    (ModelNode*) &iedModel_ANALOGRW_GGIO1_AnOut1_Oper_origin_orCat,
    0,
    IEC61850_FC_CO,
    IEC61850_CONSTRUCTED,
    0,
    NULL,
    0};

DataAttribute iedModel_ANALOGRW_GGIO1_AnOut1_Oper_origin_orCat = {
    DataAttributeModelType,
    "orCat",
    (ModelNode*) &iedModel_ANALOGRW_GGIO1_AnOut1_Oper_origin,
    (ModelNode*) &iedModel_ANALOGRW_GGIO1_AnOut1_Oper_origin_orIdent,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

DataAttribute iedModel_ANALOGRW_GGIO1_AnOut1_Oper_origin_orIdent = {
    DataAttributeModelType,
    "orIdent",
    (ModelNode*) &iedModel_ANALOGRW_GGIO1_AnOut1_Oper_origin,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_OCTET_STRING_64,
    0,
    NULL,
    0};

DataAttribute iedModel_ANALOGRW_GGIO1_AnOut1_Oper_ctlNum = {
    DataAttributeModelType,
    "ctlNum",
    (ModelNode*) &iedModel_ANALOGRW_GGIO1_AnOut1_Oper,
    (ModelNode*) &iedModel_ANALOGRW_GGIO1_AnOut1_Oper_T,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_INT8U,
    0,
    NULL,
    0};

DataAttribute iedModel_ANALOGRW_GGIO1_AnOut1_Oper_T = {
    DataAttributeModelType,
    "T",
    (ModelNode*) &iedModel_ANALOGRW_GGIO1_AnOut1_Oper,
    (ModelNode*) &iedModel_ANALOGRW_GGIO1_AnOut1_Oper_Test,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_TIMESTAMP,
    0,
    NULL,
    0};

DataAttribute iedModel_ANALOGRW_GGIO1_AnOut1_Oper_Test = {
    DataAttributeModelType,
    "Test",
    (ModelNode*) &iedModel_ANALOGRW_GGIO1_AnOut1_Oper,
    (ModelNode*) &iedModel_ANALOGRW_GGIO1_AnOut1_Oper_Check,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_BOOLEAN,
    0,
    NULL,
    0};

DataAttribute iedModel_ANALOGRW_GGIO1_AnOut1_Oper_Check = {
    DataAttributeModelType,
    "Check",
    (ModelNode*) &iedModel_ANALOGRW_GGIO1_AnOut1_Oper,
    NULL,
    NULL,
    0,
    IEC61850_FC_CO,
    IEC61850_CHECK,
    0,
    NULL,
    0};

DataAttribute iedModel_ANALOGRW_GGIO1_AnOut1_ctlModel = {
    DataAttributeModelType,
    "ctlModel",
    (ModelNode*) &iedModel_ANALOGRW_GGIO1_AnOut1,
    NULL,
    NULL,
    0,
    IEC61850_FC_CF,
    IEC61850_ENUMERATED,
    0,
    NULL,
    0};

extern ReportControlBlock iedModel_ANALOGRW_LLN0_report0;

ReportControlBlock iedModel_ANALOGRW_LLN0_report0 = {&iedModel_ANALOGRW_LLN0, "ANALOGRW_RCB01", "analogrw_rpt", false, "analogrw_dataset", 1, 24, 175, 50, 1000, {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0}, NULL};


extern GSEControlBlock iedModel_ANALOGRW_LLN0_gse0;

static PhyComAddress iedModel_ANALOGRW_LLN0_gse0_address = {
  4,
  0,
  4096,
  {0xc4, 0xde, 0xe2, 0xda, 0x9, 0xe7}
};

GSEControlBlock iedModel_ANALOGRW_LLN0_gse0 = {&iedModel_ANALOGRW_LLN0, "gcbAnalogs", "analogs", "analogrw_dataset", 2, false, &iedModel_ANALOGRW_LLN0_gse0_address, 1000, 3000, NULL};





IedModel iedModel = {
    "pub",
    &iedModel_ANALOGRW,
    &iedModelds_ANALOGRW_LLN0_analogrw_dataset,
    &iedModel_ANALOGRW_LLN0_report0,
    &iedModel_ANALOGRW_LLN0_gse0,
    NULL,
    NULL,
    NULL,
    NULL,
    initializeValues
};

static void
initializeValues()
{

iedModel_ANALOGRW_LLN0_Mod_stVal.mmsValue = MmsValue_newIntegerFromInt32(5);

iedModel_ANALOGRW_LLN0_Mod_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(0);

iedModel_ANALOGRW_GGIO1_Mod_stVal.mmsValue = MmsValue_newIntegerFromInt32(1);

iedModel_ANALOGRW_GGIO1_Mod_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(0);

iedModel_ANALOGRW_GGIO1_AnOut1_ctlModel.mmsValue = MmsValue_newIntegerFromInt32(1);
}
