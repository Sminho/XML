C++에서 MSXML을 사용하여 XML을 생성하고, 이를 예쁘게 포맷팅하기 위해 .xsl 스타일시트를 사용하는 방법을 설명하겠습니다. 먼저, XML을 생성한 후, XSLT를 사용하여 원하는 형태로 변환하는 과정입니다.

1. XML 생성
먼저, MSXML4를 사용하여 XML을 생성하는 방법입니다.

cpp
코드 복사
```
#include <iostream>
#include <comdef.h>
#include <msxml2.h>

#pragma comment(lib, "msxml2.lib")

void CreateXML(IXMLDOMDocument* pXMLDoc)
{
    // 루트 엘리먼트 생성
    IXMLDOMElement* pRootElem = nullptr;
    pXMLDoc->createElement(_bstr_t("root"), &pRootElem);

    // 자식 엘리먼트 생성
    for (int i = 1; i <= 3; ++i)
    {
        IXMLDOMElement* pElem = nullptr;
        pXMLDoc->createElement(_bstr_t("element"), &pElem);

        // 텍스트 노드 추가
        IXMLDOMText* pTextNode = nullptr;
        pXMLDoc->createTextNode(_bstr_t(std::to_wstring(i).c_str()), &pTextNode);
        pElem->appendChild(pTextNode, nullptr);

        // 루트 엘리먼트에 자식 엘리먼트 추가
        pRootElem->appendChild(pElem, nullptr);

        // 릴리스
        pElem->Release();
        pTextNode->Release();
    }

    // 문서에 루트 엘리먼트 추가
    pXMLDoc->appendChild(pRootElem, nullptr);
    pRootElem->Release();
}

int main()
{
    CoInitialize(NULL);

    IXMLDOMDocument* pXMLDoc = nullptr;
    HRESULT hr = CoCreateInstance(CLSID_DOMDocument, nullptr, CLSCTX_INPROC_SERVER, IID_IXMLDOMDocument, (void**)&pXMLDoc);

    if (SUCCEEDED(hr) && pXMLDoc)
    {
        CreateXML(pXMLDoc);

        // 생성한 XML 문서 저장
        pXMLDoc->save(_variant_t("output.xml"));

        pXMLDoc->Release();
    }

    CoUninitialize();
    return 0;
}
```
2. XSL 파일 작성
아래는 XML을 예쁘게 포맷팅하는 XSL 파일 예제입니다. 이 파일을 format.xsl로 저장합니다.

xml
코드 복사
```
<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
    <xsl:output method="xml" indent="yes"/>
    <xsl:strip-space elements="*"/>

    <xsl:template match="/">
        <xsl:apply-templates/>
    </xsl:template>

    <xsl:template match="*">
        <xsl:copy>
            <xsl:apply-templates select="@* | node()"/>
        </xsl:copy>
    </xsl:template>
</xsl:stylesheet>
```
3. XML 변환
이제, MSXML을 사용하여 XML을 XSL을 통해 변환하는 코드를 작성합니다.

cpp
코드 복사
```
#include <iostream>
#include <comdef.h>
#include <msxml2.h>

#pragma comment(lib, "msxml2.lib")

void CreateXML(IXMLDOMDocument* pXMLDoc)
{
    // 루트 엘리먼트 생성
    IXMLDOMElement* pRootElem = nullptr;
    pXMLDoc->createElement(_bstr_t("root"), &pRootElem);

    // 자식 엘리먼트 생성
    for (int i = 1; i <= 3; ++i)
    {
        IXMLDOMElement* pElem = nullptr;
        pXMLDoc->createElement(_bstr_t("element"), &pElem);

        // 텍스트 노드 추가
        IXMLDOMText* pTextNode = nullptr;
        pXMLDoc->createTextNode(_bstr_t(std::to_wstring(i).c_str()), &pTextNode);
        pElem->appendChild(pTextNode, nullptr);

        // 루트 엘리먼트에 자식 엘리먼트 추가
        pRootElem->appendChild(pElem, nullptr);

        // 릴리스
        pElem->Release();
        pTextNode->Release();
    }

    // 문서에 루트 엘리먼트 추가
    pXMLDoc->appendChild(pRootElem, nullptr);
    pRootElem->Release();
}

void TransformXML(IXMLDOMDocument* pXMLDoc)
{
    // XSL 문서 로드
    IXMLDOMDocument* pXSLDoc = nullptr;
    CoCreateInstance(CLSID_DOMDocument, nullptr, CLSCTX_INPROC_SERVER, IID_IXMLDOMDocument, (void**)&pXSLDoc);

    VARIANT_BOOL loadSuccess;
    pXSLDoc->load(_variant_t("format.xsl"), &loadSuccess);

    if (loadSuccess == VARIANT_TRUE)
    {
        // 변환 수행
        IXMLDOMDocument* pOutputDoc = nullptr;
        CoCreateInstance(CLSID_DOMDocument, nullptr, CLSCTX_INPROC_SERVER, IID_IXMLDOMDocument, (void**)&pOutputDoc);

        pXMLDoc->transformNodeToObject(pXSLDoc, pOutputDoc);

        // 변환된 XML 문서 저장
        pOutputDoc->save(_variant_t("formatted_output.xml"));
        pOutputDoc->Release();
    }

    pXSLDoc->Release();
}

int main()
{
    CoInitialize(NULL);

    IXMLDOMDocument* pXMLDoc = nullptr;
    HRESULT hr = CoCreateInstance(CLSID_DOMDocument, nullptr, CLSCTX_INPROC_SERVER, IID_IXMLDOMDocument, (void**)&pXMLDoc);

    if (SUCCEEDED(hr) && pXMLDoc)
    {
        CreateXML(pXMLDoc);

        // 생성한 XML 문서 저장
        pXMLDoc->save(_variant_t("output.xml"));

        // 변환 수행
        TransformXML(pXMLDoc);

        pXMLDoc->Release();
    }

    CoUninitialize();
    return 0;
}
```
이 코드는 C++에서 MSXML을 사용하여 XML 문서를 생성하고, .xsl 파일을 통해 예쁘게 포맷팅된 XML 문서를 출력하는 방법을 보여줍니다. output.xml 파일이 생성되고, format.xsl 파일을 통해 formatted_output.xml로 포맷팅된 XML 파일이 생성됩니다.
