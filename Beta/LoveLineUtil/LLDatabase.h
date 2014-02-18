

#pragma once


#include <windows.h>
#include <msxml.h>
#include "global.h"
#include "ccstring.h"
#include "CCLog.h"
#include "LLTypes.h"


class LLEpisode
{
public:
    GUID guid;
    CCString FileName;
    DWORD dwYear;
    DWORD dwMonth;
    DWORD dwDay;

    CCString Guests;
    CCString Description;
};


class LLCommercial
{
public:
    GUID guid;
    DWORD dwIn;
    DWORD dwOut;

    GUID guidEpisode;// Parent
};


class LLLink
{
public:
    DWORD dwPos;
    GUID guid;
    GUID guidEpisode;// Parent
    GUID guidFeature;
};


class LLFeature
{
public:
    GUID guid;
    CCString Title;
    CCString Text;
};


typedef HRESULT (CALLBACK* ENUMEPISODESPROC)(LLEpisode* pFeature, PVOID pUser);
typedef HRESULT (CALLBACK* ENUMEPISODESPROC2)(LLLink* pl, LLEpisode* pe, PVOID pUser);
typedef HRESULT (CALLBACK* ENUMCOMMERCIALSPROC)(LLCommercial* pFeature, PVOID pUser);
typedef HRESULT (CALLBACK* ENUMFEATURESPROC)(LLFeature* pFeature, PVOID pUser);
typedef HRESULT (CALLBACK* ENUMFEATURESPROC2)(LLLink* pl, LLFeature* pFeature, PVOID pUser);
typedef HRESULT (CALLBACK* ENUMLINKSPROC)(LLLink* pFeature, PVOID pUser);


class CCEXPORT LLDatabase
{
public:

    LLDatabase();
    ~LLDatabase();

    HRESULT Open(CCString FileName, CCLog* pErrText);
    HRESULT Close();

    /*
        The database also holds the last play positions
    */
    HRESULT SetLastPosition(GUID& id, DWORD dwMS, CCLog* pLog);
    HRESULT GetLastPosition(GUID* id, DWORD* pdwMS, CCLog* pLog);

    /*
        And also holds settings.
    */
    HRESULT GetSetting(CCString Name, CCString* pValue, CCLog* pLog);
    HRESULT SetSetting(CCString Name, CCString Value, CCLog* pLog);

    // Adds new stuff and grabs a new ID for them to ensure that
    // it will be unique.
    HRESULT AddEpisode(LLEpisode* pEpisode, CCLog* pErrText);
    HRESULT AddCommercial(LLCommercial* pCommercial, CCLog* pErrText);
    HRESULT AddFeature(LLFeature* pFeature, CCLog* pErrText);
    HRESULT AddLink(LLLink* pLink, CCLog* pErrText);

    // Enumerations directly out of the database
    HRESULT EnumCommercials(GUID& Episode, ENUMCOMMERCIALSPROC pProc, PVOID pUser, CCLog* pErrText);
    HRESULT EnumEpisodes(ENUMEPISODESPROC pProc, PVOID pUser, CCLog* pErrText);
    HRESULT EnumFeatures(ENUMFEATURESPROC pProc, PVOID pUser, CCLog* pErrText);
    HRESULT EnumLinks(ENUMLINKSPROC pProc, PVOID pUser, CCLog* pErrText);

    /*
        Enumerates all the features that this episode contains.
    */
    HRESULT EnumFeatures(GUID& Episode, ENUMFEATURESPROC2 pProc, PVOID pUser, CCLog* pErrText);

    /*
        Enumerates all the episodes that this feature references.
    */
    HRESULT EnumEpisodes(GUID& Feature, ENUMEPISODESPROC2 pProc, PVOID pUser, CCLog* pLog);

    HRESULT GetFeature(GUID& Feature, LLFeature* pData, CCLog* pLog);
    HRESULT GetEpisode(GUID& Episode, LLEpisode* pData, CCLog* pLog);
    HRESULT GetCommercial(GUID& id, LLCommercial* p, CCLog* pLog);
    HRESULT GetLink(GUID& id, LLLink* p, CCLog* pLog);

    HRESULT SetEpisode(LLEpisode* pEpisode, CCLog* pLog);
    HRESULT SetFeature(LLFeature* pFeature, CCLog* pLog);
    HRESULT SetLink(LLLink* pLink, CCLog* pLog);
    HRESULT SetCommercial(LLCommercial* pCommercial, CCLog* pLog);

    HRESULT RemoveEpisode(GUID& id, CCLog* pLog);
    HRESULT RemoveLink(GUID& id, CCLog* pLog);
    HRESULT RemoveFeature(GUID& id, CCLog* pLog);
    HRESULT RemoveCommercial(GUID& id, CCLog* pLog);

    /*
        Finds the closest episode that occurs AFTER the specified date.
    */
    HRESULT GetNextEpisode(LLTIME Date, GUID* pOut, CCLog* pLog);

    /*
        Checks for duplicates, etc...
    */
    BOOL IsValidFeatureTitle(CCString sTitle);

    /*
        Pass in NOT the full path.
    */
    BOOL IsFileEnlisted(CCString sFileName);

private:

    // Used in GetNextEpisode()
    static HRESULT CALLBACK EnumEpisodeProc(LLEpisode* pe, PVOID pUser);
    LLTIME lltTargetDate;
    LLTIME lltClosestDate;
    GUID   idResultingEpisode;

    // Returns the "Position" node.
    IXMLDOMNode* _GetPosition(CCLog* pLog);

    HRESULT _EpisodeFromNode(LLEpisode*, IXMLDOMNode* pNode, CCLog* pErrText);
    HRESULT _CommercialFromNode(LLCommercial*, IXMLDOMNode* pNode, CCLog* pErrText);
    HRESULT _LinkFromNode(LLLink*, IXMLDOMNode* pNode, CCLog* pErrText);
    HRESULT _FeatureFromNode(LLFeature*, IXMLDOMNode* pNode, CCLog* pErrText);

    IXMLDOMNode* _GetEpisode(GUID& Episode, CCLog* pErrText);
    IXMLDOMNode* _GetFeature(GUID& Feature, CCLog* pErrText);
    IXMLDOMNode* _GetLink(GUID& Link, CCLog* pErrText);
    IXMLDOMNode* _GetCommercial(GUID& Commercial, CCLog* pErrText);

    HRESULT _GetAttribute(IXMLDOMElement* pNode, CCString Name, CCString* pValue, CCLog* pErrText);
    HRESULT _SetAttribute(IXMLDOMElement* pNode, CCString Name, CCString Value, CCLog* pLog);
    HRESULT _AppendChild(IXMLDOMNode* pNewParent, IXMLDOMNode* pNewChild, CCLog* pErrText);
    HRESULT _AppendText(IXMLDOMNode* pNode, CCString Text, CCLog* pErrText);
    DWORD _GetParents(IXMLDOMNode* pNode);
    IXMLDOMElement* _NodeToElement(IXMLDOMNode** ppNode);// This releases the node, too.

    // Adds new stuff but doesn't check for IDs.
    HRESULT _AddEpisode(LLEpisode* pEpisode, CCLog* pErrText);
    HRESULT _AddCommercial(LLCommercial* pCommercial, CCLog* pErrText);
    HRESULT _AddFeature(LLFeature* pFeature, CCLog* pErrText);
    HRESULT _AddLink(LLLink* pLink, CCLog* pErrText);

    HRESULT _RemoveNode(IXMLDOMNode* pNode, CCLog* pLog);

    HRESULT _CreateNewSettingElement(CCString Name, IXMLDOMNode** ppOutNode, CCLog* pLog);

    CCString FileName;
    IXMLDOMNode* pDatabase;// <LoveLine>
    IXMLDOMDocument* pDoc;

};




