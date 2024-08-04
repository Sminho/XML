#include <iostream>
#include <chrono>

// MSXML 라이브러리 헤더
#include <windows.h>
#include <msxml6.h>
#pragma comment(lib, "msxml6.lib")

// PugiXML 라이브러리 헤더
#include "pugixml.hpp"

// CMarkup 라이브러리 헤더
#include "CMarkup.h"

// MSXML 테스트 함수
void TestMSXML()
{
    auto start = std::chrono::high_resolution_clock::now();

    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr))
    {
        std::cout << "Failed to initialize COM library.\n";
        return;
    }

    IXMLDOMDocument *pXMLDoc = NULL;
    hr = CoCreateInstance(__uuidof(DOMDocument60), NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pXMLDoc));
    if (FAILED(hr))
    {
        std::cout << "Failed to create XML DOM instance.\n";
        CoUninitialize();
        return;
    }

    pXMLDoc->put_async(VARIANT_FALSE);
    pXMLDoc->put_validateOnParse(VARIANT_FALSE);
    pXMLDoc->put_resolveExternals(VARIANT_FALSE);

    IXMLDOMElement *pRoot = NULL;
    pXMLDoc->createElement(L"root", &pRoot);
    pXMLDoc->appendChild(pRoot, NULL);

    for (int i = 0; i < 300000; ++i)
    {
        IXMLDOMElement *pElement = NULL;
        pXMLDoc->createElement(L"element", &pElement);
        wchar_t buffer[16];
        swprintf_s(buffer, L"%d", i);
        pElement->put_text(buffer);
        pRoot->appendChild(pElement, NULL);
    }

    VARIANT varFileName;
    VariantInit(&varFileName);
    varFileName.vt = VT_BSTR;
    varFileName.bstrVal = SysAllocString(L"msxml_output.xml");

    hr = pXMLDoc->save(varFileName);
    if (FAILED(hr))
    {
        std::cout << "Failed to save XML document.\n";
    }

    pXMLDoc->Release();
    VariantClear(&varFileName);
    CoUninitialize();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "MSXML time: " << elapsed.count() << " seconds\n";
}

// PugiXML 테스트 함수
void TestPugiXML()
{
    auto start = std::chrono::high_resolution_clock::now();

    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("root");

    for (int i = 0; i < 300000; ++i)
    {
        pugi::xml_node node = root.append_child("element");
        node.append_child(pugi::node_pcdata).set_value(std::to_string(i).c_str());
    }

    if (!doc.save_file("pugixml_output.xml"))
    {
        std::cout << "Failed to save XML document.\n";
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "PugiXML time: " << elapsed.count() << " seconds\n";
}

// CMarkup 테스트 함수
void TestCMarkup()
{
    auto start = std::chrono::high_resolution_clock::now();

    CMarkup xml;
    xml.AddElem("root");
    xml.IntoElem();

    for (int i = 0; i < 300000; ++i)
    {
        xml.AddElem("element", std::to_string(i));
    }

    if (!xml.Save("cmarkup_output.xml"))
    {
        std::cout << "Failed to save XML document.\n";
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "CMarkup time: " << elapsed.count() << " seconds\n";
}

int main()
{
    std::cout << "Testing MSXML...\n";
    TestMSXML();

    std::cout << "Testing PugiXML...\n";
    TestPugiXML();

    std::cout << "Testing CMarkup...\n";
    TestCMarkup();

    return 0;
}
