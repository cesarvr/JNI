package pdf;
import java.io.ByteArrayInputStream;
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

  public void html2pdf(String htmlText) {
    OutputStream file;
    try {
      file = new FileOutputStream(new File("Test.pdf"));

      Document document = new Document();
      PdfWriter writer = PdfWriter.getInstance(document, file);
      document.open();
      InputStream is = new ByteArrayInputStream(htmlText.getBytes());
      XMLWorkerHelper.getInstance().parseXHtml(writer, document, is);
      document.close();
      file.close();

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
  }

  public static void main(String[] args) {
    System.out.println("Generating PDF");
    new P2HService().html2pdf("");
  }
}
