import javax.crypto.*;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;
import java.io.ByteArrayOutputStream;
import java.security.InvalidAlgorithmParameterException;
import java.security.InvalidKeyException;
import java.security.NoSuchAlgorithmException;
import java.util.Arrays;

public class Message {

    final static String _MacType = "HmacSHA256";
    public static byte[] encrypt(byte[] macType, byte[] messageArr, byte[] typeIV, byte[] typeOfEncrypt) throws NoSuchAlgorithmException, InvalidKeyException, NoSuchPaddingException, InvalidAlgorithmParameterException, IllegalBlockSizeException, BadPaddingException {

        Mac mac = Mac.getInstance(_MacType);
        SecretKeySpec secretKeySpec = new SecretKeySpec(macType, _MacType);
        mac.init(secretKeySpec);
        byte[] messageMac = mac.doFinal(messageArr);

        ByteArrayOutputStream finalMessageMac = new ByteArrayOutputStream();
        finalMessageMac.writeBytes(messageArr);
        finalMessageMac.writeBytes(messageMac);

        Cipher cipher = Cipher.getInstance("AES/CBC/PKCS5Padding");
        IvParameterSpec ivParameterSpec = new IvParameterSpec(typeIV);
        SecretKeySpec cipherSpec = new SecretKeySpec(typeOfEncrypt, "AES");

        cipher.init(Cipher.ENCRYPT_MODE, cipherSpec, ivParameterSpec);

        return cipher.doFinal(finalMessageMac.toByteArray());

    }
    public static byte[] decrypt(byte[] messageArr, byte[] typeIV, byte[] typeOfEncrypt) throws NoSuchAlgorithmException, InvalidKeyException, NoSuchPaddingException, InvalidAlgorithmParameterException, IllegalBlockSizeException, BadPaddingException {

        Cipher cipher = Cipher.getInstance("AES/CBC/PKCS5Padding");
        IvParameterSpec ivParameterSpec = new IvParameterSpec(typeIV);
        SecretKeySpec cipherSpec = new SecretKeySpec(typeOfEncrypt, "AES");

        cipher.init(Cipher.DECRYPT_MODE, cipherSpec, ivParameterSpec);

        byte[] result = cipher.doFinal(messageArr);



        return Arrays.copyOf(result, result.length - 32);
    }
}
