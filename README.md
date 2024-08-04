# C++에서 XML 파서 비교: MSXML, PugiXML, CMarkup

## 1. MSXML (Microsoft XML Core Services)
https://learn.microsoft.com/en-us/previous-versions/windows/desktop/ms763742(v=vs.85)#in-this-documentation

### 특징
- **Microsoft 제공**: MSXML은 Microsoft에서 제공하는 XML 파서로, Windows 운영 체제와 통합되어있습니다.
- **COM 기반**: COM(Component Object Model) 기반으로 설계되어 있어 COM 인터페이스를 통해 XML 문서를 Parsing합니다.
- **DOM 및 SAX 지원**: DOM(Document Object Model) 및 SAX(Simple API for XML) 파싱 모드를 모두 지원합니다.
- **XSLT 및 XPath 지원**: XSLT(Extensible Stylesheet Language Transformations)와 XPath(XML Path Language)를 지원하여 XML 문서의 변환 및 쿼리가 가능합니다.

### 장점
- **Windows 통합**: Windows 환경에서 OS와 통합되어, Visual Studio와 같은 Microsoft 개발 도구와의 호환성이 좋습니다.
- **풍부한 기능**: XSLT, XPath와 같은 고급 기능을 지원하여 복잡한 XML 처리 가능합니다.

### 단점
- **플랫폼 종속적**: MSXML은 Windows 전용으로, 다른 운영 체제에서는 사용할 수 없습니다.
- **COM 복잡성**: COM 인터페이스를 사용하는 경우 XML 처리에는 과도하게 복잡성이 올라가고,
                  특히 COM 호출 오버헤드로 인해 성능이 저하될 수 있습니다.

### C++에서 MSXML을 사용하여 XML 저장 및 불러오기

1. 초기 설정
먼저, MSXML 라이브러리를 사용하기 위해 필요한 초기 설정

```cpp
#include <iostream>
#include <comdef.h>
#include <msxml6.h>

// COM 라이브러리 초기화
if (FAILED(CoInitialize(NULL))) {
    std::cerr << "COM 라이브러리 초기화 실패" << std::endl;
    return -1;
}

// COM 라이브러리 해제
CoUninitialize();
```
2. XML 문서 생성 및 저장
XML 문서를 생성하고 파일로 저장하는 함수입니다.

XML 문서 생성
```cpp
HRESULT CreateXMLDocument(IXMLDOMDocument** ppDoc) {
    HRESULT hr = CoCreateInstance(CLSID_DOMDocument60, NULL, CLSCTX_INPROC_SERVER, IID_IXMLDOMDocument, (void**)ppDoc);
    if (FAILED(hr)) {
        std::cerr << "XML 문서 생성 실패" << std::endl;
    }
    return hr;
}
```
루트 엘리먼트 생성 및 추가
```cpp
HRESULT CreateRootElement(IXMLDOMDocument* pDoc, IXMLDOMElement** ppRoot) {
    HRESULT hr = pDoc->createElement(L"Root", ppRoot);
    if (SUCCEEDED(hr)) {
        pDoc->appendChild(*ppRoot, NULL);
    } else {
        std::cerr << "루트 엘리먼트 생성 실패" << std::endl;
    }
    return hr;
}
```
자식 엘리먼트 생성 및 추가
```cpp
HRESULT AddChildElement(IXMLDOMDocument* pDoc, IXMLDOMElement* pParent, const wchar_t* tagName, const wchar_t* text) {
    IXMLDOMElement* pChild = NULL;
    HRESULT hr = pDoc->createElement(tagName, &pChild);
    if (SUCCEEDED(hr)) {
        IXMLDOMText* pText = NULL;
        hr = pDoc->createTextNode(text, &pText);
        if (SUCCEEDED(hr)) {
            pChild->appendChild(pText, NULL);
            pParent->appendChild(pChild, NULL);
            pText->Release();
        }
        pChild->Release();
    }
    return hr;
}
```
XML 문서 저장
```cpp
HRESULT SaveXMLDocument(IXMLDOMDocument* pDoc, const wchar_t* filePath) {
    VARIANT varFileName;
    VariantInit(&varFileName);
    varFileName.vt = VT_BSTR;
    varFileName.bstrVal = SysAllocString(filePath);

    HRESULT hr = pDoc->save(varFileName);
    if (FAILED(hr)) {
        std::cerr << "XML 문서 저장 실패" << std::endl;
    }

    VariantClear(&varFileName);
    return hr;
}
```
전체 예제 코드
```cpp
#include <iostream>
#include <comdef.h>
#include <msxml6.h>

HRESULT CreateXMLDocument(IXMLDOMDocument** ppDoc);
HRESULT CreateRootElement(IXMLDOMDocument* pDoc, IXMLDOMElement** ppRoot);
HRESULT AddChildElement(IXMLDOMDocument* pDoc, IXMLDOMElement* pParent, const wchar_t* tagName, const wchar_t* text);
HRESULT SaveXMLDocument(IXMLDOMDocument* pDoc, const wchar_t* filePath);

int main() {
    if (FAILED(CoInitialize(NULL))) {
        std::cerr << "COM 라이브러리 초기화 실패" << std::endl;
        return -1;
    }

    IXMLDOMDocument* pDoc = NULL;
    if (SUCCEEDED(CreateXMLDocument(&pDoc))) {
        IXMLDOMElement* pRoot = NULL;
        if (SUCCEEDED(CreateRootElement(pDoc, &pRoot))) {
            AddChildElement(pDoc, pRoot, L"Child1", L"Value1");
            AddChildElement(pDoc, pRoot, L"Child2", L"Value2");

            SaveXMLDocument(pDoc, L"example.xml");
            pRoot->Release();
        }
        pDoc->Release();
    }

    CoUninitialize();
    return 0;
}

HRESULT CreateXMLDocument(IXMLDOMDocument** ppDoc) {
    HRESULT hr = CoCreateInstance(CLSID_DOMDocument60, NULL, CLSCTX_INPROC_SERVER, IID_IXMLDOMDocument, (void**)ppDoc);
    if (FAILED(hr)) {
        std::cerr << "XML 문서 생성 실패" << std::endl;
    }
    return hr;
}

HRESULT CreateRootElement(IXMLDOMDocument* pDoc, IXMLDOMElement** ppRoot) {
    HRESULT hr = pDoc->createElement(L"Root", ppRoot);
    if (SUCCEEDED(hr)) {
        pDoc->appendChild(*ppRoot, NULL);
    } else {
        std::cerr << "루트 엘리먼트 생성 실패" << std::endl;
    }
    return hr;
}

HRESULT AddChildElement(IXMLDOMDocument* pDoc, IXMLDOMElement* pParent, const wchar_t* tagName, const wchar_t* text) {
    IXMLDOMElement* pChild = NULL;
    HRESULT hr = pDoc->createElement(tagName, &pChild);
    if (SUCCEEDED(hr)) {
        IXMLDOMText* pText = NULL;
        hr = pDoc->createTextNode(text, &pText);
        if (SUCCEEDED(hr)) {
            pChild->appendChild(pText, NULL);
            pParent->appendChild(pChild, NULL);
            pText->Release();
        }
        pChild->Release();
    }
    return hr;
}

HRESULT SaveXMLDocument(IXMLDOMDocument* pDoc, const wchar_t* filePath) {
    VARIANT varFileName;
    VariantInit(&varFileName);
    varFileName.vt = VT_BSTR;
    varFileName.bstrVal = SysAllocString(filePath);

    HRESULT hr = pDoc->save(varFileName);
    if (FAILED(hr)) {
        std::cerr << "XML 문서 저장 실패" << std::endl;
    }

    VariantClear(&varFileName);
    return hr;
}
```
3. XML 문서 불러오기
저장된 XML 문서를 불러오는 함수입니다.

XML 문서 로드
```cpp
HRESULT LoadXMLDocument(IXMLDOMDocument* pDoc, const wchar_t* filePath) {
    VARIANT varFileName;
    VariantInit(&varFileName);
    varFileName.vt = VT_BSTR;
    varFileName.bstrVal = SysAllocString(filePath);

    VARIANT_BOOL loadSuccess;
    HRESULT hr = pDoc->load(varFileName, &loadSuccess);
    if (FAILED(hr) || loadSuccess == VARIANT_FALSE) {
        std::cerr << "XML 문서 로드 실패" << std::endl;
    }

    VariantClear(&varFileName);
    return hr;
}
```
XML 데이터 접근
```cpp
void PrintXMLDocument(IXMLDOMDocument* pDoc) {
    IXMLDOMElement* pRoot = NULL;
    pDoc->get_documentElement(&pRoot);
    if (pRoot) {
        IXMLDOMNodeList* pChildNodes = NULL;
        pRoot->get_childNodes(&pChildNodes);
        if (pChildNodes) {
            long length;
            pChildNodes->get_length(&length);
            for (long i = 0; i < length; ++i) {
                IXMLDOMNode* pNode = NULL;
                pChildNodes->get_item(i, &pNode);
                if (pNode) {
                    BSTR nodeName;
                    pNode->get_nodeName(&nodeName);
                    std::wcout << L"Node Name: " << nodeName << std::endl;
                    SysFreeString(nodeName);

                    BSTR nodeText;
                    pNode->get_text(&nodeText);
                    std::wcout << L"Node Text: " << nodeText << std::endl;
                    SysFreeString(nodeText);

                    pNode->Release();
                }
            }
            pChildNodes->Release();
        }
        pRoot->Release();
    }
}
```
전체 예제 코드
```cpp
#include <iostream>
#include <comdef.h>
#include <msxml6.h>

HRESULT CreateXMLDocument(IXMLDOMDocument** ppDoc);
HRESULT LoadXMLDocument(IXMLDOMDocument* pDoc, const wchar_t* filePath);
void PrintXMLDocument(IXMLDOMDocument* pDoc);

int main() {
    if (FAILED(CoInitialize(NULL))) {
        std::cerr << "COM 라이브러리 초기화 실패" << std::endl;
        return -1;
    }

    IXMLDOMDocument* pDoc = NULL;
    if (SUCCEEDED(CreateXMLDocument(&pDoc))) {
        if (SUCCEEDED(LoadXMLDocument(pDoc, L"example.xml"))) {
            PrintXMLDocument(pDoc);
        }
        pDoc->Release();
    }

    CoUninitialize();
    return 0;
}

HRESULT CreateXMLDocument(IXMLDOMDocument** ppDoc) {
    HRESULT hr = CoCreateInstance(CLSID_DOMDocument60, NULL, CLSCTX_INPROC_SERVER, IID_IXMLDOMDocument, (void**)ppDoc);
    if (FAILED(hr)) {
        std::cerr << "XML 문서 생성 실패" << std::endl;
    }
    return hr;
}

HRESULT LoadXMLDocument(IXMLDOMDocument* pDoc, const wchar_t* filePath) {
    VARIANT varFileName;
    VariantInit(&varFileName);
    varFileName.vt = VT_BSTR;
    varFileName.bstrVal = SysAllocString(filePath);

    VARIANT_BOOL loadSuccess;
    HRESULT hr = pDoc->load(varFileName, &loadSuccess);
    if (FAILED(hr) || loadSuccess == VARIANT_FALSE) {
        std::cerr << "XML 문서 로드 실패" << std::endl;
    }

    VariantClear(&varFileName);
    return hr;
}

void PrintXMLDocument(IXMLDOMDocument* pDoc) {
    IXMLDOMElement* pRoot = NULL;
    pDoc->get_documentElement(&pRoot);
    if (pRoot) {
        IXMLDOMNodeList* pChildNodes = NULL;
        pRoot->get_childNodes(&pChildNodes);
        if (pChildNodes) {
            long length;
            pChildNodes->get_length(&length);
            for (long i = 0; i < length; ++i) {
                IXMLDOMNode* pNode = NULL;
                pChildNodes->get_item(i, &pNode);
                if (pNode) {
                    BSTR nodeName;
                    pNode->get_nodeName(&nodeName);
                    std::wcout << L"Node Name: " << nodeName << std::endl;
                    SysFreeString(nodeName);

                    BSTR nodeText;
                    pNode->get_text(&nodeText);
                    std::wcout << L"Node Text: " << nodeText << std::endl;
                    SysFreeString(nodeText);

                    pNode->Release();
                }
            }
            pChildNodes->Release();
        }
        pRoot->Release();
    }
}
```
## 2. PugiXML
https://pugixml.org/

### 특징
- **경량화**: PugiXML은 경량화된 XML 파서로, 성능과 사용의 용이성에 중점을 두고 설계되었습니다.
- **DOM 기반**: DOM 기반의 파서로, XML 문서를 메모리에 로드하고 조작할 수 있습니다.
- **간단한 API**: 간단하고 직관적인 API를 제공하여 사용하기 쉽습니다.
- **C++ 표준 라이브러리 사용**: C++ 표준 라이브러리를 사용하며, 추가적인 종속성이 없습니다.

### 장점
- **사용 용이성**: 직관적인 API로 쉽게 배울 수 있고 사용할 수 있습니다.
- **플랫폼 독립적**: Windows, Linux, macOS 등 다양한 운영 체제에서 사용할 수 있습니다.
- **빠른 성능**: 경량화되어 있어 빠른 성능을 제공합니다.
- **무료 및 오픈 소스**: MIT 라이선스 하에 배포되어 상업적 용도로도 무료로 사용할 수 있습니다.
- **XPath 지원**: XPath 쿼리를 지원하여 XML 문서의 특정 부분을 쉽게 검색할 수 있습니다.

### 단점
- **기능 제한**: 기본적인 XML 처리에는 적합하지만, XSLT와 같은 고급 기능은 지원하지 않습니다.

### C++에서 PugiXML을 사용하여 XML 저장 및 불러오기

1. PugiXML 설치

```cpp
#include "pugixml.hpp"
```
2. XML 문서 생성 및 저장
XML 문서 생성
```cpp
pugi::xml_document doc;
pugi::xml_node root = doc.append_child("Root");

// 자식 엘리먼트 추가
pugi::xml_node child1 = root.append_child("Child1");
child1.append_child(pugi::node_pcdata).set_value("Value1");

pugi::xml_node child2 = root.append_child("Child2");
child2.append_child(pugi::node_pcdata).set_value("Value2");

// XML 문서를 파일로 저장
if (!doc.save_file("example.xml")) {
    std::cerr << "XML 문서 저장 실패" << std::endl;
}
```
전체 예제 코드
```cpp
#include "pugixml.hpp"
#include <iostream>

int main() {
    pugi::xml_document doc;
    pugi::xml_node root = doc.append_child("Root");

    // 자식 엘리먼트 추가
    pugi::xml_node child1 = root.append_child("Child1");
    child1.append_child(pugi::node_pcdata).set_value("Value1");

    pugi::xml_node child2 = root.append_child("Child2");
    child2.append_child(pugi::node_pcdata).set_value("Value2");

    // XML 문서를 파일로 저장
    if (!doc.save_file("example.xml")) {
        std::cerr << "XML 문서 저장 실패" << std::endl;
        return -1;
    }

    std::cout << "XML 문서가 성공적으로 저장되었습니다." << std::endl;
    return 0;
}
```
3. XML 문서 불러오기
XML 문서 로드
```cpp
pugi::xml_document doc;
pugi::xml_parse_result result = doc.load_file("example.xml");

if (!result) {
    std::cerr << "XML 문서 로드 실패: " << result.description() << std::endl;
}
```
XML 데이터 접근
```cpp
pugi::xml_node root = doc.child("Root");

for (pugi::xml_node child : root.children()) {
    std::cout << "Node Name: " << child.name() << std::endl;
    std::cout << "Node Value: " << child.child_value() << std::endl;
}
```
전체 예제 코드
```cpp
#include "pugixml.hpp"
#include <iostream>

int main() {
    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_file("example.xml");

    if (!result) {
        std::cerr << "XML 문서 로드 실패: " << result.description() << std::endl;
        return -1;
    }

    pugi::xml_node root = doc.child("Root");

    for (pugi::xml_node child : root.children()) {
        std::cout << "Node Name: " << child.name() << std::endl;
        std::cout << "Node Value: " << child.child_value() << std::endl;
    }

    return 0;
}
```

## 3. CMarkup
http://www.firstobject.com/index.html

### 특징
- **경량화**: CMarkup도 경량화된 XML 파서로, 간단한 XML 처리를 목표로 설계되었습니다.
- **DOM 및 텍스트 기반 파싱**: DOM 기반 파싱뿐만 아니라, 텍스트 기반 파싱도 지원하여 XML 문서를 조작할 수 있습니다.
- **C++ 클래스**: 순수 C++ 클래스 기반으로 설계되어 있습니다.

### 장점
- **경량화**: 메모리 사용량이 적고, 성능이 빠릅니다.
- **간단한 사용**: 간단한 API로 XML 문서의 읽기 및 쓰기가 용이합니다.
- **상용 라이브러리**: 안정성과 지원을 제공하는 상용 라이브러리로, 특정 요구 사항에 맞게 커스터마이즈 가능합니다.

### 단점
- **기능 제한**: 복잡한 XML 처리에는 적합하지 않을 수 있으며, 고급 기능이 부족할 수 있습니다.
- **상용 라이브러리**: 무료 라이브러리가 아니므로 상용 라이선스가 필요합니다.
- **XPath 미지원**: XPath 쿼리를 지원하지 않습니다.

## 요약

- **MSXML**: Windows 환경에서 강력하고 다양한 기능을 필요로 할 때 적합합니다. 하지만 플랫폼 종속적이고 COM 복잡성을 가집니다.
- **PugiXML**: 경량화와 성능, 사용의 용이성을 중시할 때 적합합니다. 무료로 사용할 수 있으며, XPath 쿼리를 지원합니다. 하지만 고급 기능은 부족합니다.
- **CMarkup**: 경량화된 상용 라이브러리로, 간단한 XML 처리가 필요할 때 적합합니다. 상용 라이선스가 필요하고 기능이 제한적일 수 있으며, XPath를 지원하지 않습니다.

