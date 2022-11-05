# Microsoft Office Word doc format C library

### Word and .Doc Files

The binary format for Microsoft Word 97 and later versions is based on a structure referred to as a .doc file or "compound file". 
Compound File Binary format is documented at
http://download.microsoft.com/download/0/B/E/0BE8BDD7-E5E8-422A-ABFD-4342ED7AD8 86/WindowsCompoundBinaryFileFormatSpecification.pdf.

A Word .doc file consists of a:
- Main stream
- Summary information stream
- Table stream
- Data stream
- Custom XML storage (Added in Word 2007)
- 0 or more object streams which contain private data for OLE 2.0 objects embedded within the Word document

The summary information stream is described in the Format of the Summary Info Stream
in a Word File section.
The object stream contains binary data for embedded objects. Word has no knowledge of the contents ofthis stream.
