#include <iostream>
#include <libxml/parser.h>
#include <libxml/xmlschemas.h>

// Sprawdzenie argumentow - ściezki do pliku XML i XSD.
int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Użycie: " << argv[0] << " <plik.xml> <schemat.xsd>\n";
        return 1;
    }
    // Pobranie sciezek do plikow
    const char* xmlPath = argv[1];
    const char* xsdPath = argv[2];

    // Inicjalizacja parsera - przygotowanie kontekstu do parsowania schematu xsd
    xmlInitParser();
    xmlSchemaParserCtxtPtr pctxt = xmlSchemaNewParserCtxt(xsdPath);
    if (pctxt == nullptr) {
        std::cerr << "Nie można utworzyć kontekstu parsera XSD\n";
        return 1;
    }

    // Parsowanie schematu
    xmlSchemaPtr schema = xmlSchemaParse(pctxt);
    if (schema == nullptr) {
        std::cerr << "Błąd parsowania schematu XSD\n";
        xmlSchemaFreeParserCtxt(pctxt);
        return 1;
    }
    xmlSchemaFreeParserCtxt(pctxt);

    // Utworzenie kontekstu walidacji - potrzebny do walidacji xml wzgledem xsd
    xmlSchemaValidCtxtPtr vctxt = xmlSchemaNewValidCtxt(schema);
    if (vctxt == nullptr) {
        std::cerr << "Nie można utworzyć kontekstu walidacji\n";
        xmlSchemaFree(schema);
        return 1;
    }

    // Walidacja
    // ret == 0 - wszystko OK
    // ret > 0 - plik nie spełnia reguł
    // ret <0 - błąd samej walidacji (np. brak pliku)
    int ret = xmlSchemaValidateFile(vctxt, xmlPath, 0);
    if (ret == 0) {
        std::cout << "Walidacja powiodła się.\n";
    } else if (ret > 0) {
        std::cerr << "Plik XML niezgodny ze schematem XSD.\n";
    } else {
        std::cerr << "Podczas walidacji wystąpił błąd.\n";
    }

    // Sprzątanie
    xmlSchemaFreeValidCtxt(vctxt);
    xmlSchemaFree(schema);
    xmlCleanupParser();
    return (ret == 0) ? 0 : 1;
}

