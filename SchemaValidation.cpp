#include <iostream>
#include <comdef.h>
#include <msxml6.h>
#pragma comment(lib, "msxml6.lib")

bool MSXML_Validate(const std::wstring& xmlFilename, const std::wstring& xsdFilename) {
    CoInitialize(NULL);
    CComPtr<IXMLDOMDocument2> spXMLDoc;
    HRESULT hr = spXMLDoc.CoCreateInstance(__uuidof(DOMDocument60), NULL, CLSCTX_INPROC_SERVER);

    if (FAILED(hr)) {
        CoUninitialize();
        return false;
    }

    spXMLDoc->put_async(VARIANT_FALSE);
    spXMLDoc->put_validateOnParse(VARIANT_TRUE);

    VARIANT_BOOL vbSuccess;
    CComBSTR bstrXSD(xsdFilename.c_str());
    CComBSTR bstrXSDCache(L"MSXML2.XMLSchemaCache.6.0");
    CComPtr<IXMLDOMSchemaCollection2> spSchemaCache;
    hr = spSchemaCache.CoCreateInstance(bstrXSDCache);
    if (FAILED(hr)) {
        CoUninitialize();
        return false;
    }

    hr = spSchemaCache->add(bstrXSD, _variant_t(xsdFilename.c_str()));
    if (FAILED(hr)) {
        CoUninitialize();
        return false;
    }

    spXMLDoc->putref_schemas(_variant_t((IDispatch*)spSchemaCache));
    hr = spXMLDoc->load(_variant_t(xmlFilename.c_str()), &vbSuccess);

    if (vbSuccess == VARIANT_TRUE) {
        std::cout << "MSXML: XML is valid" << std::endl;
        CoUninitialize();
        return true;
    } else {
        std::cout << "MSXML: XML is invalid" << std::endl;
        CComPtr<IXMLDOMParseError> spError;
        spXMLDoc->get_parseError(&spError);
        CComBSTR reason;
        spError->get_reason(&reason);
        std::wcout << reason << std::endl;
        CoUninitialize();
        return false;
    }
}

int main() {
    std::wstring xmlFilename = L"example.xml";
    std::wstring xsdFilename = L"example.xsd";

    MSXML_Validate(xmlFilename, xsdFilename);

    return 0;
}
