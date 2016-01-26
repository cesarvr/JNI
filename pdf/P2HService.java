package pdf;

import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import com.itextpdf.text.Document;
import com.itextpdf.text.DocumentException;
import com.itextpdf.text.pdf.PdfWriter;
import com.itextpdf.tool.xml.XMLWorkerHelper;

public class P2HService {

  public byte[] html2pdf(String str) {
    ByteArrayOutputStream ostream = null;
    
    try {
      ostream = new ByteArrayOutputStream();
      
      Document document = new Document();
      PdfWriter writer = PdfWriter.getInstance(document, ostream);
      document.open();
      InputStream is = new ByteArrayInputStream(str.getBytes());
      XMLWorkerHelper.getInstance().parseXHtml(writer, document, is);
      document.close();
      ostream.close();

      return ostream.toByteArray();
      
    } catch (FileNotFoundException e) {
      // TODO Auto-generated catch block
      e.printStackTrace();
    } catch (IOException e) {
      // TODO Auto-generated catch block
      e.printStackTrace();
    } catch (DocumentException e) {
      // TODO Auto-generated catch block
      e.printStackTrace();
    }
    
    return ostream.toByteArray();
  }

  public static void main(String[] args) {
    System.out.println("Generating PDF");
    new P2HService().html2pdf("<html><body> This is my Project </body></html>");
  }

}
