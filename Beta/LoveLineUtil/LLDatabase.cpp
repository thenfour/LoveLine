

#include "LLDatabase.h"
#include "Global.h"
#include "traceerrorsys.h"
#include <shlwapi.h>


/*

<LoveLine>
    <Setting name="SelectedPlugin" value=""/>
    <Position id="{234234234-2345-2344-4234-3423424}" Time="454543"/>
    <Episode Guests="" Date="10/24/01" Description="" FileName="" ID="234234-234-234-234-234234234">
        <Commercial Start="234444" End="23444" ID=""/>
        <Link Feature="{234234234-2345-2344-4234-3423424}" ID=""/>
    </Episode>
    <Feature Description="" Title="" ID=""/>
</LoveLine>

*/


LLDatabase::LLDatabase()
{
    this->pDatabase = NULL;
    this->pDoc = NULL;
}


LLDatabase::~LLDatabase()
{
    this->Close();
}


HRESULT LLDatabase::Open(CCString FileName, CCLog* pErrText)
{
    HRESULT hr;
    this->Close();
    VARIANT_BOOL b;

    this->FileName = FileName;

    /*
        We first determine if the file already exists.  If it does, use it.
        If not, we'll create a new one.
    */
    if(PathFileExists(FileName) == FALSE)
    {
        CCString XML;
        WORD dwUnicodeHeader = 0xFEFF;
        DWORD br;

        HANDLE hFile = CreateFile(FileName, GENERIC_WRITE, FILE_SHARE_READ,
            NULL, CREATE_NEW, NULL, NULL);
        if(hFile == INVALID_HANDLE_VALUE || hFile == NULL)
        {
            pErrText->AddEntry(L"Could not create the new XML file.");
            pErrText->AddEntry(GetLastError(), L"CreateFile");
            return E_FAIL;
        }

        WriteFile(hFile, &dwUnicodeHeader, sizeof(WORD), &br, NULL);
        XML.FormatW(L"<LoveLine>\r\n</LoveLine>\r\n\r\n");
        XML.WriteToFileW(hFile);

        CloseHandle(hFile);
    }

    // Start using MSXML.
    if(FAILED(hr = CoCreateInstance(
        CLSID_DOMDocument, NULL, CLSCTX_INPROC_SERVER, IID_IXMLDOMDocument, (PVOID*)&this->pDoc)))
    {
        pErrText->AddEntry(L"Could not create MSXML objects.");
        pErrText->AddEntry(L"Ensure the MSXML dependencies are installed correctly.");
        pErrText->AddEntry(hr, L"CoCreateInstance");
        return E_FAIL;
    }

    // Open the document.
    if(FAILED(hr = this->pDoc->load((VARIANT)FileName, &b)))
    {
        pErrText->AddEntry(L"Could not load the XML file.");
        pErrText->AddEntry(hr, L"IXMLDOMDocument::load");
        SAFE_RELEASE(this->pDoc);
        return E_FAIL;
    }

    // Get the <LoveLine> node. /LoveLine
    IXMLDOMNode* pRoot = NULL;
    CCString XPath = L"/LoveLine";

    if(FAILED(hr = this->pDoc->QueryInterface(IID_IXMLDOMNode, (PVOID*)&pRoot)))
    {
        pErrText->AddEntry(L"Error working with MSXML objects.");
        pErrText->AddEntry(hr, L"IXMLDOMDocument::QueryInterface");
        SAFE_RELEASE(this->pDoc);
        return E_FAIL;
    }

    if(FAILED(hr = pRoot->selectSingleNode(XPath.bstr(), &this->pDatabase)))
    {
        pErrText->AddEntry(L"Could not get the <LoveLine> tag.  The XML document appears to be corrupt.");
        pErrText->AddEntry(hr, L"IXMLDOMNode::selectSingleNode");
        SAFE_RELEASE(pRoot);
        SAFE_RELEASE(this->pDoc);
        return E_FAIL;
    }

    SAFE_RELEASE(pRoot);

    return S_OK;
}


HRESULT LLDatabase::Close()
{
    if(this->pDoc)
    {
        this->pDoc->save(this->FileName);
    }

    SAFE_RELEASE(this->pDatabase);
    SAFE_RELEASE(this->pDoc);
    return S_OK;
}


HRESULT LLDatabase::_AddEpisode(LLEpisode* pEpisode, CCLog* pErrText)
{
    /*
        Create a new <Episode> element
        <Episode Guests="" Date="10/24/01" Description="" FileName="">
        </Episode>
    */

    IXMLDOMElement* pElement = NULL;
    IXMLDOMNode* pNode = NULL;// same as pElement, just QI'ed.
    CCString name = L"Episode";
    CCString val;
    HRESULT hr;

    if(this->pDatabase == NULL)
    {
        pErrText->AddEntry(L"The application has tried to perform operations on an uninitialized object.");
        return E_FAIL;
    }

    // If the episode already exists, we have a problem.
    if(this->IsFileEnlisted(pEpisode->FileName) == TRUE)
    {
        pErrText->AddEntry(L"The specified file already exists in the database.  There are no duplicates allowed.");
        return E_FAIL;
    }

    /*
        First create the element by itself.  We'll add it into the XML later.
    */
    if(FAILED(hr = this->pDoc->createElement((BSTR)name, &pElement)))
    {
        pErrText->AddEntry(L"Could not create a new episode in the database.");
        pErrText->AddEntry(hr, L"IXMLDOMDocument::createElement");
        return E_FAIL;
    }

    // Set the attributes
    name = L"FileName";
    if(FAILED(hr = pElement->setAttribute((BSTR)name, (VARIANT)pEpisode->FileName)))
    {
        pErrText->AddEntry(L"Could not set the FileName attribute in the new episode object.");
        pErrText->AddEntry(hr, L"IXMLDOMElement::setAttribute");
        SAFE_RELEASE(pElement);
        return E_FAIL;
    }

    name = L"Guests";
    if(FAILED(hr = pElement->setAttribute((BSTR)name, (VARIANT)pEpisode->Guests)))
    {
        pErrText->AddEntry(L"Could not set the Guests attribute in the new episode object.");
        pErrText->AddEntry(hr, L"IXMLDOMElement::setAttribute");
        SAFE_RELEASE(pElement);
        return E_FAIL;
    }

    name = L"Description";
    if(FAILED(hr = pElement->setAttribute((BSTR)name, (VARIANT)pEpisode->Description)))
    {
        pErrText->AddEntry(L"Could not set the Description attribute in the new episode object.");
        pErrText->AddEntry(hr, L"IXMLDOMElement::setAttribute");
        SAFE_RELEASE(pElement);
        return E_FAIL;
    }

    name = L"Year";
    val.FromInt(pEpisode->dwYear, 10);
    if(FAILED(hr = pElement->setAttribute((BSTR)name, (VARIANT)val)))
    {
        pErrText->AddEntry(L"Could not set the Year attribute in the new episode object.");
        pErrText->AddEntry(hr, L"IXMLDOMElement::setAttribute");
        SAFE_RELEASE(pElement);
        return E_FAIL;
    }

    name = L"Month";
    val.FromInt(pEpisode->dwMonth, 10);
    if(FAILED(hr = pElement->setAttribute((BSTR)name, (VARIANT)val)))
    {
        pErrText->AddEntry(L"Could not set the Month attribute in the new episode object.");
        pErrText->AddEntry(hr, L"IXMLDOMElement::setAttribute");
        SAFE_RELEASE(pElement);
        return E_FAIL;
    }

    name = L"Day";
    val.FromInt(pEpisode->dwDay, 10);
    if(FAILED(hr = pElement->setAttribute((BSTR)name, (VARIANT)val)))
    {
        pErrText->AddEntry(L"Could not set the Day attribute in the new episode object.");
        pErrText->AddEntry(hr, L"IXMLDOMElement::setAttribute");
        SAFE_RELEASE(pElement);
        return E_FAIL;
    }

    name = L"ID";
    val.FromGuid(&pEpisode->guid);
    if(FAILED(hr = pElement->setAttribute((BSTR)name, (VARIANT)val)))
    {
        pErrText->AddEntry(L"Could not set the ID attribute in the new episode object.");
        pErrText->AddEntry(hr, L"IXMLDOMElement::setAttribute");
        SAFE_RELEASE(pElement);
        return E_FAIL;
    }

    if(FAILED(hr = pElement->QueryInterface(IID_IXMLDOMNode, (PVOID*)&pNode)))
    {
        pErrText->AddEntry(L"Error working with MSXML objects.");
        pErrText->AddEntry(hr, L"IXMLDOMElement::QueryInterface");
        SAFE_RELEASE(pElement);
        return E_FAIL;
    }

    if(FAILED(this->_AppendChild(this->pDatabase, pNode, pErrText)))
    {
        pErrText->AddEntry(L"Could not add a new episode to the database.  Here is more detail.");
        SAFE_RELEASE(pNode);
        SAFE_RELEASE(pElement);
        return E_FAIL;
    }

    SAFE_RELEASE(pNode);
    SAFE_RELEASE(pElement);

    return S_OK;
}


HRESULT LLDatabase::_AddCommercial(LLCommercial* pCommercial, CCLog* pErrText)
{
    IXMLDOMNode* pEpisode = NULL;
    IXMLDOMElement* pCommercialElement = NULL;
    IXMLDOMNode* pCommercialNode = NULL;
    CCString val;
    CCString name;
    HRESULT hr;

    if(this->pDatabase == NULL)
    {
        pErrText->AddEntry(L"The application has tried to perform operations on an uninitialized object.");
        return E_FAIL;
    }

    // Grab the episode object.
    pEpisode = this->_GetEpisode(pCommercial->guidEpisode, pErrText);
    if(pEpisode == NULL)
    {
        val.FromGuid(&pCommercial->guidEpisode);
        pErrText->AddEntry(
            L"Episode %s not found.", (PCWSTR)val);
        return E_FAIL;
    }

    /*
        First create the element by itself.  We'll add it into the XML later.
    */
    name = L"Commercial";
    if(FAILED(hr = this->pDoc->createElement((BSTR)name, &pCommercialElement)))
    {
        pErrText->AddEntry(L"Could not create a new commercial element.");
        pErrText->AddEntry(hr, L"IXMLDOMDocument::createElement");
        return E_FAIL;
    }

    // Set the attributes
    name = L"In";
    val.FromInt(pCommercial->dwIn, 10);
    if(FAILED(hr = pCommercialElement->setAttribute((BSTR)name, (VARIANT)val)))
    {
        pErrText->AddEntry(L"Could not set the In attribute on the new commercial.");
        pErrText->AddEntry(hr, L"IXMLDOMElement::setAttribute");
        return E_FAIL;
    }

    name = L"Out";
    val.FromInt(pCommercial->dwOut, 10);
    if(FAILED(hr = pCommercialElement->setAttribute((BSTR)name, (VARIANT)val)))
    {
        pErrText->AddEntry(L"Could not set the Out attribute on the new commercial.");
        pErrText->AddEntry(hr, L"IXMLDOMElement::setAttribute");
        return E_FAIL;
    }

    name = L"ID";
    val.FromGuid(&pCommercial->guid);
    if(FAILED(hr = pCommercialElement->setAttribute((BSTR)name, (VARIANT)val)))
    {
        pErrText->AddEntry(L"Could not set the ID attribute on the new commercial.");
        pErrText->AddEntry(hr, L"IXMLDOMElement::setAttribute");
        return E_FAIL;
    }

    name = L"Episode";
    val.FromGuid(&pCommercial->guidEpisode);
    if(FAILED(hr = pCommercialElement->setAttribute((BSTR)name, (VARIANT)val)))
    {
        pErrText->AddEntry(L"Could not set the Episode attribute on the new commercial.");
        pErrText->AddEntry(hr, L"IXMLDOMElement::setAttribute");
        return E_FAIL;
    }

    if(FAILED(hr = pCommercialElement->QueryInterface(IID_IXMLDOMNode, (PVOID*)&pCommercialNode)))
    {
        pErrText->AddEntry(L"Error working with MSXML objects.");
        pErrText->AddEntry(hr, L"IXMLDOMElement::QueryInterface");
        return E_FAIL;
    }

    if(FAILED(this->_AppendChild(pEpisode, pCommercialNode, pErrText)))
    {
        pErrText->AddEntry(L"Could not add a new commercial to the database.");
        SAFE_RELEASE(pEpisode);
        SAFE_RELEASE(pCommercialNode);
        SAFE_RELEASE(pCommercialElement);
        return E_FAIL;
    }

    SAFE_RELEASE(pEpisode);
    SAFE_RELEASE(pCommercialNode);
    SAFE_RELEASE(pCommercialElement);

    return S_OK;
}


HRESULT LLDatabase::_AddFeature(LLFeature* pFeature, CCLog* pErrText)
{
    /*
        Create a new <Feature> element
        <Feature Title="" Text="" ID="">
        </Feature>
    */

    IXMLDOMElement* pElement = NULL;
    IXMLDOMNode* pNode = NULL;// same as pElement, just QI'ed.
    CCString name = L"Feature";
    CCString val;
    HRESULT hr;

    if(this->IsValidFeatureTitle(pFeature->Title) == FALSE)
    {
        pErrText->AddEntry(L"The feature title, \"%s\" is invalid.  The database was not updated.", (PCWSTR)pFeature->Title);
        return E_FAIL;
    }

    if(this->pDatabase == NULL)
    {
        pErrText->AddEntry(L"The application has tried to perform operations on an uninitialized object.");
        return E_FAIL;
    }

    /*
        First create the element by itself.  We'll add it into the XML later.
    */
    if(FAILED(hr = this->pDoc->createElement((BSTR)name, &pElement)))
    {
        pErrText->AddEntry(L"Could not create a new feature in the database.");
        pErrText->AddEntry(hr, L"IXMLDOMDocument::createElement");
        return E_FAIL;
    }

    // Set the attributes
    name = L"Title";
    if(FAILED(hr = pElement->setAttribute((BSTR)name, (VARIANT)pFeature->Title)))
    {
        pErrText->AddEntry(L"Could not set the FileName attribute on the new commercial.");
        pErrText->AddEntry(hr, L"IXMLDOMElement::setAttribute");
        SAFE_RELEASE(pElement);
        return E_FAIL;
    }

    name = L"Text";
    if(FAILED(hr = pElement->setAttribute((BSTR)name, (VARIANT)pFeature->Text)))
    {
        pErrText->AddEntry(L"Could not set the Text attribute on the new commercial.");
        pErrText->AddEntry(hr, L"IXMLDOMElement::setAttribute");
        SAFE_RELEASE(pElement);
        return E_FAIL;
    }

    name = L"ID";
    val.FromGuid(&pFeature->guid);
    if(FAILED(hr = pElement->setAttribute((BSTR)name, (VARIANT)val)))
    {
        pErrText->AddEntry(L"Could not set the ID attribute on the new commercial.");
        pErrText->AddEntry(hr, L"IXMLDOMElement::setAttribute");
        SAFE_RELEASE(pElement);
        return E_FAIL;
    }

    if(FAILED(hr = pElement->QueryInterface(IID_IXMLDOMNode, (PVOID*)&pNode)))
    {
        pErrText->AddEntry(L"Error working with MSXML objects.");
        pErrText->AddEntry(hr, L"IXMLDOMElement::QueryInterface");
        SAFE_RELEASE(pElement);
        return E_FAIL;
    }

    if(FAILED(this->_AppendChild(this->pDatabase, pNode, pErrText)))
    {
        pErrText->AddEntry(L"Could not add a new commercial to the database.");
        SAFE_RELEASE(pNode);
        SAFE_RELEASE(pElement);
        return E_FAIL;
    }

    SAFE_RELEASE(pNode);
    SAFE_RELEASE(pElement);

    return S_OK;
}


HRESULT LLDatabase::_AddLink(LLLink* pLink, CCLog* pErrText)
{
    IXMLDOMNode* pEpisode = NULL;
    IXMLDOMNode* pFeature = NULL;
    IXMLDOMElement* pLinkElement = NULL;
    IXMLDOMNode* pLinkNode = NULL;
    CCString val;
    CCString name;
    HRESULT hr;

    if(this->pDatabase == NULL)
    {
        pErrText->AddEntry(L"The application has tried to perform operations on an uninitialized object.");
        return E_FAIL;
    }

    // Make sure the feature exists.
    pFeature = this->_GetFeature(pLink->guidFeature, pErrText);
    if(pFeature == NULL)
    {
        val.FromGuid(&pLink->guidFeature);
        pErrText->AddEntry(L"Feature %s not found.", (PCWSTR)val);
        return E_FAIL;
    }

    SAFE_RELEASE(pFeature);

    // Grab the episode object.
    pEpisode = this->_GetEpisode(pLink->guidEpisode, pErrText);
    if(pEpisode == NULL)
    {
        val.FromGuid(&pLink->guidEpisode);
        pErrText->AddEntry(L"Episode %s not found.", (PCWSTR)val);
        return E_FAIL;
    }

    /*
        First create the element by itself.  We'll add it into the XML later.
    */
    name = L"Link";
    if(FAILED(hr = this->pDoc->createElement((BSTR)name, &pLinkElement)))
    {
        pErrText->AddEntry(L"Could not create a new link element.");
        pErrText->AddEntry(hr, L"IXMLDOMDocument::createElement");
        return E_FAIL;
    }

    // Set the attributes
    name = L"Episode";
    val.FromGuid(&pLink->guidEpisode);
    if(FAILED(hr = pLinkElement->setAttribute((BSTR)name, (VARIANT)val)))
    {
        pErrText->AddEntry(L"Could not set the Episode attribute on the new link.");
        pErrText->AddEntry(hr, L"IXMLDOMElement::setAttribute");
        return E_FAIL;
    }

    name = L"Feature";
    val.FromGuid(&pLink->guidFeature);
    if(FAILED(hr = pLinkElement->setAttribute((BSTR)name, (VARIANT)val)))
    {
        pErrText->AddEntry(L"Could not set the Feature attribute on the new link.");
        pErrText->AddEntry(hr, L"IXMLDOMElement::setAttribute");
        return E_FAIL;
    }

    name = L"Position";
    val.FromInt(pLink->dwPos, 10);

    if(FAILED(hr = pLinkElement->setAttribute((BSTR)name, (VARIANT)val)))
    {
        pErrText->AddEntry(L"Could not set the Position attribute on the new link.");
        pErrText->AddEntry(hr, L"IXMLDOMElement::setAttribute");
        return E_FAIL;
    }

    name = L"ID";
    val.FromGuid(&pLink->guid);
    if(FAILED(hr = pLinkElement->setAttribute((BSTR)name, (VARIANT)val)))
    {
        pErrText->AddEntry(L"Could not set the ID attribute on the new link.");
        pErrText->AddEntry(hr, L"IXMLDOMElement::setAttribute");
        return E_FAIL;
    }

    if(FAILED(hr = pLinkElement->QueryInterface(IID_IXMLDOMNode, (PVOID*)&pLinkNode)))
    {
        pErrText->AddEntry(L"Error working with MSXML objects.");
        pErrText->AddEntry(hr, L"IXMLDOMElement::QueryInterface");
        return E_FAIL;
    }

    if(FAILED(this->_AppendChild(pEpisode, pLinkNode, pErrText)))
    {
        pErrText->AddEntry(L"Could not add a new commercial to the database.");
        SAFE_RELEASE(pEpisode);
        SAFE_RELEASE(pLinkElement);
        SAFE_RELEASE(pLinkNode);
        return E_FAIL;
    }

    SAFE_RELEASE(pEpisode);
    SAFE_RELEASE(pLinkElement);
    SAFE_RELEASE(pLinkNode);

    return S_OK;
}


HRESULT LLDatabase::EnumCommercials(GUID& Episode, ENUMCOMMERCIALSPROC pProc, PVOID pUser, CCLog* pErrText)
{
    IXMLDOMNodeList* pList = NULL;
    IXMLDOMNode* pItem = NULL;
    CCString Guid;
    CCString XPath;
    LLCommercial c;
    HRESULT hr;
    long l;
    int i;

    if(this->pDatabase == NULL)
    {
        pErrText->AddEntry(L"The application has tried to perform operations on an uninitialized object.");
        return E_FAIL;
    }

    Guid.FromGuid(&Episode);
    XPath.FormatW(L"Episode[@ID=\"%s\"]/Commercial", (PCWSTR)Guid);

    if(FAILED(hr = this->pDatabase->selectNodes(XPath.bstr(), &pList)))
    {
        pErrText->AddEntry(L"Could not grab commercial objects out of the database.");
        pErrText->AddEntry(hr, L"IXMLDOMNode::selectNodes");
        return E_FAIL;
    }

    if(FAILED(hr = pList->get_length(&l)))
    {
        pErrText->AddEntry(L"Could not grab commercial objects out of the database.");
        pErrText->AddEntry(hr, L"IXMLDOMNodeList::get_length");
        SAFE_RELEASE(pList);
        return E_FAIL;
    }

    for(i=0;i<l;i++)
    {
        if(FAILED(hr = pList->nextNode(&pItem)))
        {
            // it failed, but let's just ignore it...
            continue;
        }

        // We have the episode.
        if(FAILED(hr = this->_CommercialFromNode(&c, pItem, pErrText)))
        {
            pErrText->AddEntry(L"A commercial in the database could not be read.");
            SAFE_RELEASE(pItem);
            continue;// Continue it because we don't want this one node to ruin it for all nodes.
        }
        else
        {
            pProc(&c, pUser);
        }

        SAFE_RELEASE(pItem);
    }

    SAFE_RELEASE(pList);

    return S_OK;
}


HRESULT LLDatabase::EnumEpisodes(ENUMEPISODESPROC pProc, PVOID pUser, CCLog* pErrText)
{
    IXMLDOMNodeList* pList = NULL;
    IXMLDOMNode* pItem = NULL;
    CCString XPath;
    LLEpisode e;
    HRESULT hr;
    long l;
    int i;

    if(this->pDatabase == NULL)
    {
        pErrText->AddEntry(L"The application has tried to perform operations on an uninitialized object.");
        return E_FAIL;
    }

    XPath.FormatW(L"Episode");

    if(FAILED(hr = this->pDatabase->selectNodes(XPath.bstr(), &pList)))
    {
        pErrText->AddEntry(L"Could not grab episode objects out of the database.");
        pErrText->AddEntry(hr, L"IXMLDOMNodeList::selectNodes");
        return E_FAIL;
    }

    if(FAILED(hr = pList->get_length(&l)))
    {
        pErrText->AddEntry(L"Could not grab episode objects out of the database.");
        pErrText->AddEntry(hr, L"IXMLDOMNodeList::get_length");
        SAFE_RELEASE(pList);
        return E_FAIL;
    }

    for(i=0;i<l;i++)
    {
        if(FAILED(hr = pList->nextNode(&pItem)))
        {
            // it failed, but let's just ignore it...
            continue;
        }

        // We have the episode.
        if(FAILED(hr = this->_EpisodeFromNode(&e, pItem, pErrText)))
        {
            pErrText->AddEntry(L"A episode in the database could not be read.");
            SAFE_RELEASE(pItem);
            continue;// Continue it because we don't want this one node to ruin it for all nodes.
        }
        else
        {
            pProc(&e, pUser);
        }

        SAFE_RELEASE(pItem);
    }

    SAFE_RELEASE(pList);

    return S_OK;
}


HRESULT LLDatabase::EnumFeatures(ENUMFEATURESPROC pProc, PVOID pUser, CCLog* pErrText)
{
    IXMLDOMNodeList* pList = NULL;
    IXMLDOMNode* pItem = NULL;
    CCString XPath;
    LLFeature f;
    HRESULT hr;
    long l;
    int i;

    if(this->pDatabase == NULL)
    {
        pErrText->AddEntry(L"The application has tried to perform operations on an uninitialized object.");
        return E_FAIL;
    }

    XPath.FormatW(L"Feature");

    if(FAILED(hr = this->pDatabase->selectNodes(XPath.bstr(), &pList)))
    {
        pErrText->AddEntry(L"Could not grab feature objects out of the database.");
        pErrText->AddEntry(hr, L"IXMLDOMNodeList::selectNodes");
        return E_FAIL;
    }

    if(FAILED(hr = pList->get_length(&l)))
    {
        pErrText->AddEntry(L"Could not grab feature objects out of the database.");
        pErrText->AddEntry(hr, L"IXMLDOMNodeList::get_length");
        SAFE_RELEASE(pList);
        return E_FAIL;
    }

    for(i=0;i<l;i++)
    {
        if(FAILED(hr = pList->nextNode(&pItem)))
        {
            // it failed, but let's just ignore it...
            continue;
        }

        // We have the episode.
        if(FAILED(hr = this->_FeatureFromNode(&f, pItem, pErrText)))
        {
            pErrText->AddEntry(L"A episode in the database could not be read.");
            SAFE_RELEASE(pItem);
            continue;// Continue it because we don't want this one node to ruin it for all nodes.
        }
        else
        {
            pProc(&f, pUser);
        }

        SAFE_RELEASE(pItem);
    }

    SAFE_RELEASE(pList);

    return S_OK;
}


HRESULT LLDatabase::EnumLinks(ENUMLINKSPROC pProc, PVOID pUser, CCLog* pErrText)
{
    IXMLDOMNodeList* pList = NULL;
    IXMLDOMNode* pItem = NULL;
    CCString XPath;
    LLLink Link;
    HRESULT hr;
    long l;
    int i;

    if(this->pDatabase == NULL)
    {
        pErrText->AddEntry(L"The application has tried to perform operations on an uninitialized object.");
        return E_FAIL;
    }

    XPath.FormatW(L"Episode/Link");

    if(FAILED(hr = this->pDatabase->selectNodes(XPath.bstr(), &pList)))
    {
        pErrText->AddEntry(L"Could not grab feature objects out of the database.");
        pErrText->AddEntry(hr, L"IXMLDOMNodeList::selectNodes");
        return E_FAIL;
    }

    if(FAILED(hr = pList->get_length(&l)))
    {
        pErrText->AddEntry(L"Could not grab feature objects out of the database.");
        pErrText->AddEntry(hr, L"IXMLDOMNodeList::get_length");
        SAFE_RELEASE(pList);
        return E_FAIL;
    }

    for(i=0;i<l;i++)
    {
        if(FAILED(hr = pList->nextNode(&pItem)))
        {
            // it failed, but let's just ignore it...
            continue;
        }

        // We have the episode.
        if(FAILED(hr = this->_LinkFromNode(&Link, pItem, pErrText)))
        {
            pErrText->AddEntry(L"A episode in the database could not be read.");
            SAFE_RELEASE(pItem);
            continue;// Continue it because we don't want this one node to ruin it for all nodes.
        }
        else
        {
            pProc(&Link, pUser);
        }

        SAFE_RELEASE(pItem);
    }

    SAFE_RELEASE(pList);

    return S_OK;
}


HRESULT LLDatabase::EnumEpisodes(GUID& Feature, ENUMEPISODESPROC2 pProc, PVOID pUser, CCLog* pLog)
{
    IXMLDOMNodeList* pList = NULL;
    IXMLDOMNode* pItem = NULL;
    IXMLDOMNode* pEpisode = NULL;
    CCString XPath;
    CCString Guid;
    LLLink Link;
    LLEpisode Episode;
    HRESULT hr;
    long l;
    int i;

    if(this->pDatabase == NULL)
    {
        pLog->AddEntry(L"The application has tried to perform operations on an uninitialized object.");
        //return E_FAIL;
    }

    // Select the link elements that have the specified feature
    Guid.FromGuid(&Feature);
    XPath.FormatW(L"Episode/Link[@Feature = \"%s\"]", (PCWSTR)Guid);


    if(FAILED(hr = this->pDatabase->selectNodes(XPath.bstr(), &pList)))
    {
        pLog->AddEntry(L"Could not grab link objects out of the database.");
        pLog->AddEntry(hr, L"IXMLDOMNodeList::selectNodes");
        //return E_FAIL;
    }

    if(FAILED(hr = pList->get_length(&l)))
    {
        pLog->AddEntry(L"Could not grab link objects out of the database.");
        pLog->AddEntry(hr, L"IXMLDOMNodeList::get_length");
        SAFE_RELEASE(pList);
        //return E_FAIL;
    }

    for(i=0;i<l;i++)
    {
        if(FAILED(hr = pList->nextNode(&pItem)))
        {
            // it failed, but let's just ignore it...
            continue;
        }

        // We have the link.  Now select the episode by GUID.
        if(FAILED(hr = this->_LinkFromNode(&Link, pItem, pLog)))
        {
            pLog->AddEntry(L"A link in the database could not be read.");
            SAFE_RELEASE(pItem);
            continue;// Continue it because we don't want this one node to ruin it for all nodes.
        }
        else
        {
            // Now select the episode.
            pEpisode = this->_GetEpisode(Link.guidEpisode, pLog);
            if(pEpisode == NULL)
            {
                // Couldn't select the feature.
                SAFE_RELEASE(pItem);
                continue;
            }
            else
            {
                if(FAILED(this->_EpisodeFromNode(&Episode, pEpisode, pLog)))
                {
                    // malformed episode.
                    SAFE_RELEASE(pEpisode);
                    SAFE_RELEASE(pItem);
                    continue;
                }

                pProc(&Link, &Episode, pUser);

                SAFE_RELEASE(pEpisode);
            }
        }

        SAFE_RELEASE(pItem);
    }

    SAFE_RELEASE(pList);

    return S_OK;
}


HRESULT LLDatabase::EnumFeatures(GUID& Episode, ENUMFEATURESPROC2 pProc, PVOID pUser, CCLog* pErrText)
{
    IXMLDOMNodeList* pList = NULL;
    IXMLDOMNode* pItem = NULL;
    IXMLDOMNode* pFeature = NULL;
    CCString Guid;
    CCString XPath;
    LLFeature f;
    LLLink Link;
    HRESULT hr;
    long l;
    int i;

    if(this->pDatabase == NULL)
    {
        pErrText->AddEntry(L"The application has tried to perform operations on an uninitialized object.");
        return E_FAIL;
    }

    Guid.FromGuid(&Episode);
    XPath.FormatW(L"Episode[@ID=\"%s\"]/Link", (PCWSTR)Guid);

    if(FAILED(hr = this->pDatabase->selectNodes(XPath.bstr(), &pList)))
    {
        pErrText->AddEntry(L"Could not grab link objects out of the database.");
        pErrText->AddEntry(hr, L"IXMLDOMNodeList::selectNodes");
        return E_FAIL;
    }

    if(FAILED(hr = pList->get_length(&l)))
    {
        pErrText->AddEntry(L"Could not grab link objects out of the database.");
        pErrText->AddEntry(hr, L"IXMLDOMNodeList::get_length");
        SAFE_RELEASE(pList);
        return E_FAIL;
    }

    for(i=0;i<l;i++)
    {
        if(FAILED(hr = pList->nextNode(&pItem)))
        {
            // it failed, but let's just ignore it...
            continue;
        }

        // We have the link.  Now select the feature by GUID.
        if(FAILED(hr = this->_LinkFromNode(&Link, pItem, pErrText)))
        {
            pErrText->AddEntry(L"A commercial in the database could not be read.");
            SAFE_RELEASE(pItem);
            continue;// Continue it because we don't want this one node to ruin it for all nodes.
        }
        else
        {
            // Now select the feature.
            pFeature = this->_GetFeature(Link.guidFeature, pErrText);
            if(pFeature == NULL)
            {
                // Couldn't select the feature.
                SAFE_RELEASE(pItem);
                continue;
            }
            else
            {
                if(FAILED(this->_FeatureFromNode(&f, pFeature, pErrText)))
                {
                    // malformed feature.
                    SAFE_RELEASE(pFeature);
                    SAFE_RELEASE(pItem);
                    continue;
                }

                pProc(&Link, &f, pUser);

                SAFE_RELEASE(pFeature);
            }
        }

        SAFE_RELEASE(pItem);
    }

    SAFE_RELEASE(pList);

    return S_OK;
}


IXMLDOMNode* LLDatabase::_GetLink(GUID& Link, CCLog* pErrText)
{
    IXMLDOMNode* pNode = NULL;
    CCString XPath;
    CCString Guid;
    HRESULT hr;

    if(this->pDatabase == NULL)
    {
        pErrText->AddEntry(L"The application has tried to perform operations on an uninitialized object.");
        return NULL;
    }

    // Form the XPath query.
    Guid.FromGuid(&Link);
    XPath.FormatW(L"Episode/Link[@ID = \"%s\"]", (PCWSTR)Guid);

    if(FAILED(hr = pDatabase->selectSingleNode(XPath.bstr(), &pNode)))
    {
        pErrText->AddEntry(L"Could not find the specified link in the database.");
        pErrText->AddEntry(hr, L"IXMLDOMNode::selectSingleNode");
        return NULL;
    }

    return pNode;
}


IXMLDOMNode* LLDatabase::_GetCommercial(GUID& Commercial, CCLog* pErrText)
{
    IXMLDOMNode* pNode = NULL;
    CCString XPath;
    CCString Guid;
    HRESULT hr;

    if(this->pDatabase == NULL)
    {
        pErrText->AddEntry(L"The application has tried to perform operations on an uninitialized object.");
        return NULL;
    }

    // Form the XPath query.
    Guid.FromGuid(&Commercial);
    XPath.FormatW(L"Episode/Commercial[@ID = \"%s\"]", (PCWSTR)Guid);

    if(FAILED(hr = pDatabase->selectSingleNode(XPath.bstr(), &pNode)))
    {
        pErrText->AddEntry(L"Could not find the specified commercial in the database.");
        pErrText->AddEntry(hr, L"IXMLDOMNode::selectSingleNode");
        return NULL;
    }

    return pNode;
}


IXMLDOMNode* LLDatabase::_GetFeature(GUID& Feature, CCLog* pErrText)
{
    IXMLDOMNode* pNode = NULL;
    CCString XPath;
    CCString Guid;
    HRESULT hr;

    if(this->pDatabase == NULL)
    {
        pErrText->AddEntry(L"The application has tried to perform operations on an uninitialized object.");
        return NULL;
    }

    // Form the XPath query.
    Guid.FromGuid(&Feature);
    XPath.FormatW(L"Feature[@ID = \"%s\"]", (PCWSTR)Guid);

    if(FAILED(hr = pDatabase->selectSingleNode(XPath.bstr(), &pNode)))
    {
        pErrText->AddEntry(L"Could not find the specified feature in the database.");
        pErrText->AddEntry(hr, L"IXMLDOMNode::selectSingleNode");
        return NULL;
    }

    return pNode;
}


IXMLDOMNode* LLDatabase::_GetEpisode(GUID& Episode, CCLog* pErrText)
{
    IXMLDOMNode* pNode = NULL;
    CCString XPath;
    CCString Guid;
    HRESULT hr;

    if(this->pDatabase == NULL)
    {
        pErrText->AddEntry(L"The application has tried to perform operations on an uninitialized object.");
        return NULL;
    }

    // Form the XPath query.
    Guid.FromGuid(&Episode);
    XPath.FormatW(L"Episode[@ID = \"%s\"]", (PCWSTR)Guid);

    if(FAILED(hr = pDatabase->selectSingleNode(XPath.bstr(), &pNode)))
    {
        pErrText->AddEntry(L"Could not find the specified episode in the database.");
        pErrText->AddEntry(hr, L"IXMLDOMNode::selectSingleNode");
        return NULL;
    }

    return pNode;
}


/*
    This function appends a child to the end of the specified parent.
    This function is special, though, because it will form the XML well.

    Without this function, the XML gets ugly:
    <LoveLine><Episode adsfasdfasdfad=""/></LoveLine>

    With this function, it's pretty:
    <LoveLine>
        <Episode asdfasdfadsf="">
        </Episode>
    </LoveLine>
*/
HRESULT LLDatabase::_AppendChild(IXMLDOMNode* pNewParent, IXMLDOMNode* pNewChild, CCLog* pErrText)
{
    HRESULT hr;
    DWORD i;
    CCString ErrBuf;
    CCString sText;

    IXMLDOMNode* pNewOne = NULL;

    if(pNewParent == NULL || pNewChild == NULL)
    {
        pErrText->AddEntry(L"The application has tried to perform operations on an uninitialized object.");
        return E_FAIL;
    }

    DWORD dwParents = 1;// How many parents the NEW node has (parents the PARENT has + 1).
    dwParents = this->_GetParents(pNewParent) + 1;

    // append the first new line and tabs.
    sText = L"\r\n";

    for(i=1;i<dwParents;i++)
    {
        sText.cat(L"\t");
    }

    if(FAILED(hr = this->_AppendText(pNewParent, sText, pErrText)))
    {
        pErrText->AddEntry(L"Could not add a new node to the XML database.");
        return E_FAIL;
    }

    if(FAILED(hr = this->_AppendText(pNewChild, sText, pErrText)))
    {
        pErrText->AddEntry(L"Could not add a new node to the XML database.");
        return E_FAIL;
    }

    // Now add it to the XML.
    if(FAILED(hr = pNewParent->appendChild(pNewChild, &pNewOne)))
    {
        pErrText->AddEntry(L"Could not add a new node to the XML database.");
        pErrText->AddEntry(hr, L"IXMLDOMNode::appendChild");
        return E_FAIL;
    }

    sText = L"\r\n";

    for(i=1;i<dwParents - 1;i++)
    {
        sText.cat(L"\t");
    }

    if(FAILED(hr = this->_AppendText(pNewParent, sText, pErrText)))
    {
        pErrText->AddEntry(L"Could not add a new node to the XML database.");
        SAFE_RELEASE(pNewOne);
        return E_FAIL;
    }

    SAFE_RELEASE(pNewOne);

    return S_OK;
}


/*
    Returns the number of parents this node has.
*/
DWORD LLDatabase::_GetParents(IXMLDOMNode* pNode)
{
    DWORD dwParents = 0;
    IXMLDOMNode* pParent = NULL;
    IXMLDOMNode* pOldParent = NULL;

    if(pNode == NULL)
    {
        return E_FAIL;
    }

    // Because we will release this when we're doing parents, we need to addref.
    pNode->AddRef();

    // Determine how many parents the node has.  For each parent, indent it one tab character.
    pOldParent = pNode;
    while(1)
    {
        pOldParent->get_parentNode(&pParent);
        SAFE_RELEASE(pOldParent);

        if(pParent == NULL) break;
        dwParents ++;
        pOldParent = pParent;
    }

    SAFE_RELEASE(pParent);

    return dwParents;
}


/*
    Appends text to the end of an element.
*/
HRESULT LLDatabase::_AppendText(IXMLDOMNode* pNode, CCString Text, CCLog* pErrText)
{
    IXMLDOMText* pText = NULL;
    IXMLDOMNode* pTextNode = NULL;
    IXMLDOMNode* pNewOne = NULL;
    HRESULT hr;

    if(this->pDoc == NULL || pNode == NULL)
    {
        pErrText->AddEntry(L"The application has tried to perform operations on an uninitialized object.");
        return E_FAIL;
    }

    if(FAILED(hr = this->pDoc->createTextNode(Text.bstr(), &pText)))
    {
        pErrText->AddEntry(L"Could not create a new text node.");
        pErrText->AddEntry(hr, L"IXMLDOMDocument::createTextNode");
        return E_FAIL;
    }

    if(FAILED(hr = pText->QueryInterface(IID_IXMLDOMNode, (PVOID*)&pTextNode)))
    {
        pErrText->AddEntry(L"Error working with XML objects.");
        pErrText->AddEntry(hr, L"IXMLDOMText::QueryInterface");
        SAFE_RELEASE(pText);
        return E_FAIL;
    }

    // Now add it to the XML.
    if(FAILED(hr = pNode->appendChild(pTextNode, &pNewOne)))
    {
        pErrText->AddEntry(L"Error adding text to an XML node.");
        pErrText->AddEntry(hr, L"IXMLDOMNode::appendChild");
        SAFE_RELEASE(pTextNode);
        SAFE_RELEASE(pText);
        return E_FAIL;
    }

    SAFE_RELEASE(pText);
    SAFE_RELEASE(pTextNode);
    SAFE_RELEASE(pNewOne);

    return S_OK;
}


HRESULT LLDatabase::_GetAttribute(IXMLDOMElement* pNode, CCString Name, CCString* pValue, CCLog* pErrText)
{
    VARIANT v;
    HRESULT hr;

    if(FAILED(hr = pNode->getAttribute(Name.bstr(), &v)))
    {
        pErrText->AddEntry(L"Error reading the '%s' attribute.", (PCWSTR)Name);
        pErrText->AddEntry(hr, L"IXMLDOMElement::getAttribute");
        return E_FAIL;
    }

    if(hr == S_FALSE)
    {
        pErrText->AddEntry(L"The '%s' attribute does not exist.", (PCWSTR)Name);
        return E_FAIL;
    }

    if(v.vt == VT_NULL)
    {
        pErrText->AddEntry(L"The '%s' attribute is an invalid datatype.", (PCWSTR)Name);
        return E_FAIL;
    }

    *pValue = v.bstrVal;
    SysFreeString(v.bstrVal);

    return S_OK;
}


HRESULT LLDatabase::_CommercialFromNode(LLCommercial* pc, IXMLDOMNode* pNode, CCLog* pErrText)
{
    IXMLDOMElement* pElement = NULL;
    CCString v;
    HRESULT hr;

    pNode->QueryInterface(IID_IXMLDOMElement, (PVOID*)&pElement);

    if(FAILED(hr = this->_GetAttribute(pElement, L"In", &v, pErrText)))
    {
        pErrText->AddEntry(L"Couldn't read the commercial.");
        SAFE_RELEASE(pElement);
        return E_FAIL;
    }

    pc->dwIn = v.ToInt(10);

    if(FAILED(hr = this->_GetAttribute(pElement, L"Out", &v, pErrText)))
    {
        pErrText->AddEntry(L"Couldn't read the commercial.");
        SAFE_RELEASE(pElement);
        return E_FAIL;
    }

    pc->dwOut = v.ToInt(10);

    if(FAILED(hr = this->_GetAttribute(pElement, L"ID", &v, pErrText)))
    {
        pErrText->AddEntry(L"Couldn't read the commercial.");
        SAFE_RELEASE(pElement);
        return E_FAIL;
    }

    v.ToGuid(&pc->guid);

    if(FAILED(hr = this->_GetAttribute(pElement, L"Episode", &v, pErrText)))
    {
        pErrText->AddEntry(L"Couldn't read the commercial.");
        SAFE_RELEASE(pElement);
        return E_FAIL;
    }

    v.ToGuid(&pc->guidEpisode);

    SAFE_RELEASE(pElement);

    return S_OK;
}


HRESULT LLDatabase::_EpisodeFromNode(LLEpisode* pe, IXMLDOMNode* pNode, CCLog* pErrText)
{
    IXMLDOMElement* pElement = NULL;
    CCString v;
    HRESULT hr;

    pNode->QueryInterface(IID_IXMLDOMElement, (PVOID*)&pElement);

    if(FAILED(hr = this->_GetAttribute(pElement, L"Description", &v, pErrText)))
    {
        pErrText->AddEntry(L"Couldn't read the episode.");
        SAFE_RELEASE(pElement);
        return E_FAIL;
    }

    pe->Description = v;

    if(FAILED(hr = this->_GetAttribute(pElement, L"Day", &v, pErrText)))
    {
        pErrText->AddEntry(L"Couldn't read the episode.");
        SAFE_RELEASE(pElement);
        return E_FAIL;
    }

    pe->dwDay = v.ToInt(10);

    if(FAILED(hr = this->_GetAttribute(pElement, L"Month", &v, pErrText)))
    {
        pErrText->AddEntry(L"Couldn't read the episode.");
        SAFE_RELEASE(pElement);
        return E_FAIL;
    }

    pe->dwMonth = v.ToInt(10);

    if(FAILED(hr = this->_GetAttribute(pElement, L"Year", &v, pErrText)))
    {
        pErrText->AddEntry(L"Couldn't read the episode.");
        SAFE_RELEASE(pElement);
        return E_FAIL;
    }

    pe->dwYear = v.ToInt(10);

    if(FAILED(hr = this->_GetAttribute(pElement, L"FileName", &v, pErrText)))
    {
        pErrText->AddEntry(L"Couldn't read the episode.");
        SAFE_RELEASE(pElement);
        return E_FAIL;
    }

    pe->FileName = v;

    if(FAILED(hr = this->_GetAttribute(pElement, L"Guests", &v, pErrText)))
    {
        pErrText->AddEntry(L"Couldn't read the episode.");
        SAFE_RELEASE(pElement);
        return E_FAIL;
    }

    pe->Guests = v;

    if(FAILED(hr = this->_GetAttribute(pElement, L"ID", &v, pErrText)))
    {
        pErrText->AddEntry(L"Couldn't read the episode.");
        SAFE_RELEASE(pElement);
        return E_FAIL;
    }

    v.ToGuid(&pe->guid);

    SAFE_RELEASE(pElement);

    return S_OK;
}


HRESULT LLDatabase::_LinkFromNode(LLLink* pl, IXMLDOMNode* pNode, CCLog* pErrText)
{
    IXMLDOMElement* pElement = NULL;
    CCString v;
    HRESULT hr;

    pNode->QueryInterface(IID_IXMLDOMElement, (PVOID*)&pElement);

    if(FAILED(hr = this->_GetAttribute(pElement, L"Position", &v, pErrText)))
    {
        pErrText->AddEntry(L"Couldn't read the link.");
        SAFE_RELEASE(pElement);
        return E_FAIL;
    }

    pl->dwPos = v.ToInt(10);

    if(FAILED(hr = this->_GetAttribute(pElement, L"ID", &v, pErrText)))
    {
        pErrText->AddEntry(L"Couldn't read the link.");
        SAFE_RELEASE(pElement);
        return E_FAIL;
    }

    v.ToGuid(&pl->guid);

    if(FAILED(hr = this->_GetAttribute(pElement, L"Episode", &v, pErrText)))
    {
        pErrText->AddEntry(L"Couldn't read the link.");
        SAFE_RELEASE(pElement);
        return E_FAIL;
    }

    v.ToGuid(&pl->guidEpisode);

    if(FAILED(hr = this->_GetAttribute(pElement, L"Feature", &v, pErrText)))
    {
        pErrText->AddEntry(L"Couldn't read the link.");
        SAFE_RELEASE(pElement);
        return E_FAIL;
    }

    v.ToGuid(&pl->guidFeature);

    SAFE_RELEASE(pElement);

    return S_OK;
}


HRESULT LLDatabase::_FeatureFromNode(LLFeature* pf, IXMLDOMNode* pNode, CCLog* pErrText)
{
    IXMLDOMElement* pElement = NULL;
    CCString v;
    HRESULT hr;

    pNode->QueryInterface(IID_IXMLDOMElement, (PVOID*)&pElement);

    if(FAILED(hr = this->_GetAttribute(pElement, L"ID", &v, pErrText)))
    {
        pErrText->AddEntry(L"Couldn't read the feature.");
        SAFE_RELEASE(pElement);
        return E_FAIL;
    }

    v.ToGuid(&pf->guid);

    if(FAILED(hr = this->_GetAttribute(pElement, L"Text", &v, pErrText)))
    {
        pErrText->AddEntry(L"Couldn't read the feature.");
        SAFE_RELEASE(pElement);
        return E_FAIL;
    }

    pf->Text = v;

    if(FAILED(hr = this->_GetAttribute(pElement, L"Title", &v, pErrText)))
    {
        pErrText->AddEntry(L"Couldn't read the feature.");
        SAFE_RELEASE(pElement);
        return E_FAIL;
    }

    pf->Title = v;

    SAFE_RELEASE(pElement);

    return S_OK;
}


HRESULT LLDatabase::GetFeature(GUID& Feature, LLFeature* pData, CCLog* pLog)
{
    IXMLDOMNode* pFeature = NULL;
    pFeature = this->_GetFeature(Feature, pLog);

    if(pFeature == NULL)
    {
        return E_FAIL;
    }

    if(FAILED(this->_FeatureFromNode(pData, pFeature, pLog)))
    {
        SAFE_RELEASE(pFeature);
        return E_FAIL;
    }

    SAFE_RELEASE(pFeature);

    return S_OK;
}


HRESULT LLDatabase::GetEpisode(GUID& Episode, LLEpisode* pData, CCLog* pLog)
{
    IXMLDOMNode* pEpisode = NULL;
    pEpisode = this->_GetEpisode(Episode, pLog);

    if(pEpisode == NULL)
    {
        return E_FAIL;
    }

    if(FAILED(this->_EpisodeFromNode(pData, pEpisode, pLog)))
    {
        SAFE_RELEASE(pEpisode);
        return E_FAIL;
    }

    SAFE_RELEASE(pEpisode);

    return S_OK;
}


HRESULT LLDatabase::GetCommercial(GUID& id, LLCommercial* p, CCLog* pLog)
{
    IXMLDOMNode* pCommercial = NULL;
    pCommercial = this->_GetCommercial(id, pLog);

    if(pCommercial == NULL)
    {
        return E_FAIL;
    }

    if(FAILED(this->_CommercialFromNode(p, pCommercial, pLog)))
    {
        SAFE_RELEASE(pCommercial);
        return E_FAIL;
    }

    SAFE_RELEASE(pCommercial);
    return S_OK;
}


HRESULT LLDatabase::GetLink(GUID& id, LLLink* p, CCLog* pLog)
{
    IXMLDOMNode* pLink = NULL;
    pLink = this->_GetLink(id, pLog);

    if(pLink == NULL)
    {
        return E_FAIL;
    }

    if(FAILED(this->_LinkFromNode(p, pLink, pLog)))
    {
        SAFE_RELEASE(pLink);
        return E_FAIL;
    }

    SAFE_RELEASE(pLink);

    return S_OK;
}


HRESULT LLDatabase::SetEpisode(LLEpisode* pEpisode, CCLog* pLog)
{
    /*
        We can't set episodes like we set other objects...
        if we remove the episode, we'll lose all the links and commercials
        associated with it.
    */
    CCString s;
    IXMLDOMNode* pEpisodeNode = this->_GetEpisode(pEpisode->guid, pLog);
    IXMLDOMElement* pElement = NULL;

    if(pEpisodeNode == NULL)
    {
        pLog->AddEntry(L"Error while trying to access the specified episode object.");
        return E_FAIL;
    }

    pElement = this->_NodeToElement(&pEpisodeNode);
    if(!pElement)
    {
        pLog->AddEntry(L"Error while trying to access the specified episode object.");
        SAFE_RELEASE(pEpisodeNode);
        return E_FAIL;
    }

    SAFE_RELEASE(pEpisodeNode);

    /*
        If the episode already exists, we have a problem.
        We do this by checking if they're changing the filenmae.
        if they are changing the filename, we check if the new name
        already exists.  If so, exit.

        If not, we know it's already OK.
    */

    if(SUCCEEDED(this->_GetAttribute(pElement, L"FileName", &s, pLog)))
    {
        if(s.cmpi(pEpisode->FileName) != NULL)
        {
            if(this->IsFileEnlisted(pEpisode->FileName) == TRUE)
            {
                pLog->AddEntry(L"The specified file already exists in the database.  There are no duplicates allowed.");
                SAFE_RELEASE(pElement);
                return E_FAIL;
            }
        }
    }

    if(FAILED(this->_SetAttribute(pElement, L"Description", pEpisode->Description, pLog)))
    {
        pLog->AddEntry(L"Error while setting the Description attribute on the specified episode object.");
        SAFE_RELEASE(pElement);
        return E_FAIL;
    }

    if(FAILED(this->_SetAttribute(pElement, L"Guests", pEpisode->Guests, pLog)))
    {
        pLog->AddEntry(L"Error while setting the Guests attribute on the specified episode object.");
        SAFE_RELEASE(pElement);
        return E_FAIL;
    }

    if(FAILED(this->_SetAttribute(pElement, L"FileName", pEpisode->FileName, pLog)))
    {
        pLog->AddEntry(L"Error while setting the FileName attribute on the specified episode object.");
        SAFE_RELEASE(pElement);
        return E_FAIL;
    }

    s.FromInt(pEpisode->dwYear, 10);
    if(FAILED(this->_SetAttribute(pElement, L"Year", s, pLog)))
    {
        pLog->AddEntry(L"Error while setting the Year attribute on the specified episode object.");
        SAFE_RELEASE(pElement);
        return E_FAIL;
    }

    s.FromInt(pEpisode->dwMonth, 10);
    if(FAILED(this->_SetAttribute(pElement, L"Month", s, pLog)))
    {
        pLog->AddEntry(L"Error while setting the Month attribute on the specified episode object.");
        SAFE_RELEASE(pElement);
        return E_FAIL;
    }

    s.FromInt(pEpisode->dwDay, 10);
    if(FAILED(this->_SetAttribute(pElement, L"Day", s, pLog)))
    {
        pLog->AddEntry(L"Error while setting the Day attribute on the specified episode object.");
        SAFE_RELEASE(pElement);
        return E_FAIL;
    }

    s.FromGuid(&pEpisode->guid);
    if(FAILED(this->_SetAttribute(pElement, L"ID", s, pLog)))
    {
        pLog->AddEntry(L"Error while setting the ID attribute on the specified episode object.");
        SAFE_RELEASE(pElement);
        return E_FAIL;
    }


    SAFE_RELEASE(pElement);

    return S_OK;
}


HRESULT LLDatabase::SetFeature(LLFeature* pFeature, CCLog* pLog)
{
    // Remove the episode and then add it again.
    if(FAILED(this->RemoveFeature(pFeature->guid, pLog)))
    {
        return E_FAIL;
    }

    if(FAILED(this->_AddFeature(pFeature, pLog)))
    {
        return E_FAIL;
    }

    return S_OK;
}


HRESULT LLDatabase::SetLink(LLLink* pLink, CCLog* pLog)
{
    // Remove the episode and then add it again.
    if(FAILED(this->RemoveLink(pLink->guid, pLog)))
    {
        return E_FAIL;
    }

    if(FAILED(this->_AddLink(pLink, pLog)))
    {
        return E_FAIL;
    }

    return S_OK;
}


HRESULT LLDatabase::SetCommercial(LLCommercial* pCommercial, CCLog* pLog)
{
    // Remove the episode and then add it again.
    if(FAILED(this->RemoveCommercial(pCommercial->guid, pLog)))
    {
        return E_FAIL;
    }

    if(FAILED(this->_AddCommercial(pCommercial, pLog)))
    {
        return E_FAIL;
    }

    return S_OK;
}


HRESULT LLDatabase::AddEpisode(LLEpisode* pEpisode, CCLog* pErrText)
{
    UuidCreate(&pEpisode->guid);
    this->_AddEpisode(pEpisode, pErrText);
    return S_OK;
}


HRESULT LLDatabase::AddCommercial(LLCommercial* pCommercial, CCLog* pErrText)
{
    UuidCreate(&pCommercial->guid);
    this->_AddCommercial(pCommercial, pErrText);
    return S_OK;
}


HRESULT LLDatabase::AddFeature(LLFeature* pFeature, CCLog* pErrText)
{
    UuidCreate(&pFeature->guid);
    this->_AddFeature(pFeature, pErrText);
    return S_OK;
}


HRESULT LLDatabase::AddLink(LLLink* pLink, CCLog* pErrText)
{
    UuidCreate(&pLink->guid);
    this->_AddLink(pLink, pErrText);
    return S_OK;
}


HRESULT LLDatabase::_RemoveNode(IXMLDOMNode* pChild, CCLog* pLog)
{
    IXMLDOMNode* pParent = NULL;
    HRESULT hr;

    // Get the parent so we can tell it to remove the child.
    if(FAILED(hr = pChild->get_parentNode(&pParent)))
    {
        pLog->AddEntry(L"Could not get the parent node.  For this reason, the node could not be removed.");
        pLog->AddEntry(hr, L"IXMLDOMNode::get_parentNode");
        return E_FAIL;
    }

    if(FAILED(pParent->removeChild(pChild, NULL)))
    {
        pLog->AddEntry(L"Could not remove the node.");
        pLog->AddEntry(hr, L"IXMLDOMNode::removeChild");
        return E_FAIL;
    }

    return S_OK;
}


HRESULT LLDatabase::RemoveEpisode(GUID& id, CCLog* pLog)
{
    IXMLDOMNode* pNode = NULL;

    pNode = this->_GetEpisode(id, pLog);
    if(pNode == NULL)
    {
        return E_FAIL;
    }

    if(FAILED(this->_RemoveNode(pNode, pLog)))
    {
        SAFE_RELEASE(pNode);
        return E_FAIL;
    }

    SAFE_RELEASE(pNode);
    return S_OK;
}


HRESULT LLDatabase::RemoveLink(GUID& id, CCLog* pLog)
{
    IXMLDOMNode* pNode = NULL;

    pNode = this->_GetLink(id, pLog);
    if(pNode == NULL)
    {
        return E_FAIL;
    }

    if(FAILED(this->_RemoveNode(pNode, pLog)))
    {
        SAFE_RELEASE(pNode);
        return E_FAIL;
    }

    SAFE_RELEASE(pNode);
    return S_OK;
}


HRESULT LLDatabase::RemoveFeature(GUID& id, CCLog* pLog)
{
    IXMLDOMNode* pNode = NULL;

    pNode = this->_GetFeature(id, pLog);
    if(pNode == NULL)
    {
        return E_FAIL;
    }

    if(FAILED(this->_RemoveNode(pNode, pLog)))
    {
        SAFE_RELEASE(pNode);
        return E_FAIL;
    }

    SAFE_RELEASE(pNode);

    // Now the thing is here we get to delete all the links that
    // cross reference with this feature.
    CCString XPath;
    CCString idFeature;
    idFeature.FromGuid(&id);
    XPath.FormatW(L"Episode/Link[@Feature = \"%s\"]", (PCWSTR)idFeature);

    while(1)
    {
        if(FAILED(this->pDatabase->selectSingleNode(XPath, &pNode)))
        {
            // Error.
            break;
        }

        if(pNode == NULL)
        {
            // None found.
            break;
        }

        if(FAILED(this->_RemoveNode(pNode, pLog)))
        {
            break;
        }

        SAFE_RELEASE(pNode);
    }

    SAFE_RELEASE(pNode);
    return S_OK;
}


HRESULT LLDatabase::RemoveCommercial(GUID& id, CCLog* pLog)
{
    IXMLDOMNode* pNode = NULL;

    pNode = this->_GetCommercial(id, pLog);
    if(pNode == NULL)
    {
        return E_FAIL;
    }

    if(FAILED(this->_RemoveNode(pNode, pLog)))
    {
        SAFE_RELEASE(pNode);
        return E_FAIL;
    }

    SAFE_RELEASE(pNode);
    return S_OK;
}


IXMLDOMNode* LLDatabase::_GetPosition(CCLog* pLog)
{
    IXMLDOMNode* pPosition = NULL;
    CCString XPath;
    HRESULT hr;

    XPath = L"Position";
    if(FAILED(hr = this->pDatabase->selectSingleNode(XPath.bstr(), &pPosition)))
    {
        pLog->AddEntry(L"Could not get the <Position> node.");
        pLog->AddEntry(hr, L"IXMLDOMNode::selectSingleNode");
        return NULL;
    }

    return pPosition;
}


HRESULT LLDatabase::SetLastPosition(GUID& id, DWORD dwMS, CCLog* pLog)
{
    IXMLDOMNode* pPosition = NULL;
    IXMLDOMElement* pElement = NULL;
    CCString val;
    CCString name;
    HRESULT hr;

    pPosition = this->_GetPosition(pLog);

    if(pPosition == NULL)
    {
        // Try to create it.  Then give it another shot.
        name = L"Position";
        if(FAILED(hr = this->pDoc->createElement((BSTR)name, &pElement)))
        {
            pLog->AddEntry(L"Could not create a new position element in the database.");
            pLog->AddEntry(hr, L"IXMLDOMDocument::createElement");
            return E_FAIL;
        }

        if(FAILED(hr = pElement->QueryInterface(IID_IXMLDOMNode, (PVOID*)&pPosition)))
        {
            pLog->AddEntry(L"Error working with MSXML interfaces.");
            pLog->AddEntry(hr, L"IXMLDOMElement::QueryInterface");
            SAFE_RELEASE(pElement);
            return E_FAIL;
        }

        SAFE_RELEASE(pElement);

        // Now add it, empty.  We'll set the attributes later.
        if(FAILED(this->_AppendChild(this->pDatabase, pPosition, pLog)))
        {
            pLog->AddEntry(L"Could not add the position node to the database.");
            SAFE_RELEASE(pPosition);
            SAFE_RELEASE(pElement);
            return E_FAIL;
        }

        SAFE_RELEASE(pPosition);

        // Now try to start over.  We could simply call this function again,
        // but we want to avoid recursion.
        pPosition = this->_GetPosition(pLog);
        if(pPosition == NULL)
        {
            pLog->AddEntry(L"Failed to add the position element in the database.");
            return E_FAIL;
        }
    }

    if(FAILED(hr = pPosition->QueryInterface(IID_IXMLDOMElement, (PVOID*)&pElement)))
    {
        pLog->AddEntry(L"Error working with MSXML interfaces.");
        pLog->AddEntry(hr, L"IXMLDOMNode::QueryInterface");
        SAFE_RELEASE(pPosition);
        return E_FAIL;
    }

    SAFE_RELEASE(pPosition);

    // Set the attributes
    name = L"ID";
    val.FromGuid(&id);
    if(FAILED(hr = pElement->setAttribute((BSTR)name, (VARIANT)val)))
    {
        pLog->AddEntry(L"Could not set the 'ID' attribute in the position element.");
        pLog->AddEntry(hr, L"IXMLDOMElement::setAttribute");
        SAFE_RELEASE(pElement);
        return E_FAIL;
    }

    name = L"Time";
    val.FromInt(dwMS, 10);
    if(FAILED(hr = pElement->setAttribute((BSTR)name, (VARIANT)val)))
    {
        pLog->AddEntry(L"Could not set the 'Time' attribute in the position element.");
        pLog->AddEntry(hr, L"IXMLDOMElement::setAttribute");
        SAFE_RELEASE(pElement);
        return E_FAIL;
    }

    SAFE_RELEASE(pElement);

    return S_OK;
}


HRESULT LLDatabase::GetLastPosition(GUID* id, DWORD* pdwMS, CCLog* pLog)
{
    IXMLDOMNode* pPosition = NULL;
    IXMLDOMElement* pElement = NULL;
    CCString val;
    HRESULT hr;

    pPosition = this->_GetPosition(pLog);

    if(pPosition == NULL)
    {
        return E_FAIL;
    }

    if(FAILED(hr = pPosition->QueryInterface(IID_IXMLDOMElement, (PVOID*)&pElement)))
    {
        pLog->AddEntry(L"Error working with MSXML documents.");
        pLog->AddEntry(hr, L"QueryInterface(IID_IXMLDOMElement)");
        SAFE_RELEASE(pPosition);
    }

    if(FAILED(this->_GetAttribute(pElement, L"ID", &val, pLog)))
    {
        pLog->AddEntry(L"Could not get the 'ID' attribute of the <Position> element.");
        SAFE_RELEASE(pPosition);
        SAFE_RELEASE(pElement);
        return E_FAIL;
    }

    val.ToGuid(id);

    if(FAILED(this->_GetAttribute(pElement, L"Time", &val, pLog)))
    {
        pLog->AddEntry(L"Could not get the 'Time' attribute of the <Position> element.");
        SAFE_RELEASE(pElement);
        SAFE_RELEASE(pPosition);
        return E_FAIL;
    }

    *pdwMS = val.ToInt(10);

    SAFE_RELEASE(pPosition);
    SAFE_RELEASE(pElement);

    return S_OK;
}


BOOL LLDatabase::IsValidFeatureTitle(CCString sTitle)
{
    if(sTitle.len() == 0) return FALSE;

    return TRUE;
}


HRESULT LLDatabase::GetSetting(CCString Name, CCString* pValue, CCLog* pLog)
{
    IXMLDOMNode* pNode = NULL;
    IXMLDOMElement* pElement = NULL;
    CCString XPath;
    CCString Guid;
    HRESULT hr;

    if(this->pDatabase == NULL)
    {
        pLog->AddEntry(L"The application has tried to perform operations on an uninitialized object.");
        return E_FAIL;
    }

    // Form the XPath query.
    XPath.FormatW(L"Setting[@name=\"%s\"]", (PCWSTR)Name);

    if(FAILED(hr = pDatabase->selectSingleNode(XPath.bstr(), &pNode)))
    {
        // The setting doesn't exist.
        return E_FAIL;
    }

    if(pNode == NULL)
    {
        // The setting doesn't exist.
        return E_FAIL;
    }

    if(FAILED(pNode->QueryInterface(IID_IXMLDOMElement, (PVOID*)&pElement)))
    {
        SAFE_RELEASE(pNode);
        return E_FAIL;
    }

    SAFE_RELEASE(pNode);

    // Get the attribute
    if(FAILED(this->_GetAttribute(pElement, L"value", pValue, pLog)))
    {
        SAFE_RELEASE(pElement);
        return E_FAIL;
    }

    SAFE_RELEASE(pElement);

    return S_OK;
}


HRESULT LLDatabase::SetSetting(CCString Name, CCString Value, CCLog* pLog)
{
    IXMLDOMNode* pNode = NULL;
    IXMLDOMElement* pElement = NULL;
    CCString XPath;
    CCString Guid;
    HRESULT hr;

    if(this->pDatabase == NULL)
    {
        pLog->AddEntry(L"The application has tried to perform operations on an uninitialized object.");
        return E_FAIL;
    }

    // Form the XPath query.
    XPath.FormatW(L"Setting[@name=\"%s\"]", (PCWSTR)Name);

    hr = pDatabase->selectSingleNode(XPath.bstr(), &pNode);

    if(FAILED(hr) || pNode == NULL)
    {
        // The setting doesn't exist.  We have to create the node.
        if(FAILED(this->_CreateNewSettingElement(Name, &pNode, pLog)))
        {
            return E_FAIL;
        }
    }

    if(FAILED(pNode->QueryInterface(IID_IXMLDOMElement, (PVOID*)&pElement)))
    {
        SAFE_RELEASE(pNode);
        return E_FAIL;
    }

    SAFE_RELEASE(pNode);

    // Set the attribute
    XPath = L"name";
    if(FAILED(hr = pElement->setAttribute((BSTR)XPath, (VARIANT)Name)))
    {
        pLog->AddEntry(L"Could not set the %s attribute.", (PCWSTR)Name);
        pLog->AddEntry(hr, L"IXMLDOMElement::setAttribute");
        SAFE_RELEASE(pElement);
        return E_FAIL;
    }

    // Set the attribute
    XPath = L"value";
    if(FAILED(hr = pElement->setAttribute((BSTR)XPath, (VARIANT)Value)))
    {
        pLog->AddEntry(L"Could not set the %s attribute.", (PCWSTR)Value);
        pLog->AddEntry(hr, L"IXMLDOMElement::setAttribute");
        SAFE_RELEASE(pElement);
        return E_FAIL;
    }

    SAFE_RELEASE(pElement);

    return S_OK;
}


HRESULT LLDatabase::_CreateNewSettingElement(CCString Name, IXMLDOMNode** ppOutNode, CCLog* pLog)
{
    CCString ElementName;
    IXMLDOMElement* pElement = NULL;
    IXMLDOMNode* pNode = NULL;
    ElementName = L"Setting";
    HRESULT hr;

    *ppOutNode = NULL;

    if(FAILED(hr = this->pDoc->createElement(ElementName, &pElement)))
    {
        pLog->AddEntry(L"Could not create a new setting node");
        pLog->AddEntry(hr, L"IXMLDOMDocument::createElement");
        return E_FAIL;
    }

    if(FAILED(hr = pElement->QueryInterface(IID_IXMLDOMNode, (PVOID*)&pNode)))
    {
        SAFE_RELEASE(pElement);
        pLog->AddEntry(L"Error working with MSXML objects");
        pLog->AddEntry(hr, L"IXMLDOMElement::QueryInterface(IID_IXMLDOMNode)");
        return E_FAIL;
    }

    SAFE_RELEASE(pElement);

    if(FAILED(this->_AppendChild(this->pDatabase, pNode, pLog)))
    {
        SAFE_RELEASE(pNode);
        pLog->AddEntry(L"Could not insert the element into the XML document.");
        return E_FAIL;
    }

    *ppOutNode = pNode;

    //SAFE_RELEASE(pNode);

    return S_OK;
}


HRESULT LLDatabase::_SetAttribute(IXMLDOMElement* pNode, CCString Name, CCString Value, CCLog* pLog)
{
    if(FAILED(pNode->setAttribute(Name, Value)))
    {
        pLog->AddEntry(L"Could not set attribute named \"%s\" to \"%s\"", (PCWSTR)Name, (PCWSTR)Value);
        return E_FAIL;
    }

    return S_OK;
}


IXMLDOMElement* LLDatabase::_NodeToElement(IXMLDOMNode** ppNode)
{
    IXMLDOMElement* pElement = NULL;
    HRESULT hr;

    if(FAILED(hr = (*ppNode)->QueryInterface(IID_IXMLDOMElement, (PVOID*)&pElement)))
    {
        return NULL;
    }

    SAFE_RELEASE(*ppNode);

    return pElement;
}


HRESULT CALLBACK LLDatabase::EnumEpisodeProc(LLEpisode* pe, PVOID pUser)
{
    LLDatabase* pThis = (LLDatabase*)pUser;

    LLTIME lltThisTime = MAKELLTIME(pe->dwYear, pe->dwMonth, pe->dwDay);
    LLTIME lltThisDiff;
    LLTIME lltThatDiff = pThis->lltClosestDate - pThis->lltTargetDate;

    if(lltThisTime > pThis->lltTargetDate)
    {
        // We know it's AFTER... that's good.
        lltThisDiff = lltThisTime - pThis->lltTargetDate;
        if(lltThisDiff < lltThatDiff)
        {
            // We have a good match.
            pThis->lltClosestDate = lltThisTime;
            pThis->idResultingEpisode = pe->guid;
        }
    }

    return S_OK;
}



HRESULT LLDatabase::GetNextEpisode(LLTIME Date, GUID* pOut, CCLog* pLog)
{
    RPC_STATUS s;
    ZeroMemory(&this->idResultingEpisode, sizeof(GUID));
    this->lltTargetDate = Date;
    this->lltClosestDate = (LLTIME)-1;

    if(FAILED(this->EnumEpisodes(LLDatabase::EnumEpisodeProc, this, pLog)))
    {
        pLog->AddEntry(L"Error while enumerating episodes.");
        return E_FAIL;
    }

    if(UuidIsNil(&this->idResultingEpisode, &s) == TRUE)
    {
        // There were no episodes found that are after the speicfied time.
        return E_FAIL;
    }

    memcpy(pOut, &this->idResultingEpisode, sizeof(GUID));

    return S_OK;
}


BOOL LLDatabase::IsFileEnlisted(CCString sFileName)
{
    IXMLDOMNode* pNode = NULL;
    IXMLDOMElement* pElement = NULL;
    CCString XPath;
    CCString Guid;
    HRESULT hr;

    if(this->pDatabase == NULL)
    {
        return FALSE;
    }

    // Form the XPath query.
    XPath.FormatW(L"Episode[@FileName=\"%s\"]", (PCWSTR)sFileName);

    hr = pDatabase->selectSingleNode(XPath.bstr(), &pNode);

    if(FAILED(hr) || pNode == NULL)
    {
        return FALSE;
    }

    SAFE_RELEASE(pNode);

    return TRUE;
}



