import java.io.*;
import java.math.BigInteger;
import java.security.cert.Certificate;
import java.security.cert.CertificateException;
import java.security.cert.CertificateFactory;
import javax.crypto.*;
import javax.crypto.spec.IvParameterSpec;
import javax.crypto.spec.SecretKeySpec;
public class Handshake {

    final static String publicKeyString = "FFFFFFFFFFFFFFFFC90FDAA22168C234C4C6628B" +
            "80DC1CD129024E088A67CC74020BBEA63B139B22514A08798E3404DDEF9" +
            "519B3CD3A431B302B0A6DF25F14374FE1356D6D51C245E485B576625E7E" +
            "C6F44C42E9A637ED6B0BFF5CB6F406B7EDEE386BFB5A899FA5AE9F24117C" +
            "4B1FE649286651ECE45B3DC2007CB8A163BF0598DA48361C55D39A69163FA" +
            "8FD24CF5F83655D23DCA3AD961C62F356208552BB9ED529077096966D670C" +
            "354E4ABC9804F1746C08CA18217C32905E462E36CE3BE39E772C180E86039" +
            "B2783A2EC07A28FB5C55DF06F4C52C9DE2BCBF6955817183995497CEA956A" +
            "E515D2261898FA051015728E5A8AACAA68FFFFFFFFFFFFFFFF";
    final static String _MacType = "HmacSHA256";

    static BigInteger g = new BigInteger("2");
    static BigInteger n = new BigInteger(publicKeyString, 16);

    static SecretKeySpec _serverEncrypt;
    static SecretKeySpec _clientEncrypt;
    static SecretKeySpec _serverMAC;
    static SecretKeySpec _clientMAC;
    static IvParameterSpec _serverIV;
    static IvParameterSpec _clientIV;


    static Certificate getCertificate(String fileName) throws FileNotFoundException, CertificateException {
        File certificateFile = new File (fileName);
        FileInputStream certificateInputStream = new FileInputStream(certificateFile);
        CertificateFactory certificateFactory = CertificateFactory.getInstance("X.509");
        Certificate certificate = certificateFactory.generateCertificate(certificateInputStream);
        return certificate;
    }

    public static byte[] hkdfExpand(byte[] input, byte[] tag) throws Exception {
        byte[] concatenated_tag = new byte[tag.length + 1];

        for(int i = 0; i < tag.length; i++){
            concatenated_tag[i] = tag[i];
        }
        concatenated_tag[concatenated_tag.length -1] = 0x01;

        Mac hmac = Mac.getInstance(_MacType);
        hmac.init(new SecretKeySpec(input, _MacType));
        byte[] okm = hmac.doFinal(concatenated_tag);

        byte output[] = new byte[16];
        System.arraycopy(okm,0, output, 0, 16);

        return output;
    }

    static void makeSecretKeys(byte[] clientNonce, byte[] sharedSecretFromDiffieHellman) throws Exception {
        Mac hmac = Mac.getInstance(_MacType);
        hmac.init(new SecretKeySpec(sharedSecretFromDiffieHellman, _MacType));
        byte[] prk = hmac.doFinal(clientNonce);

        byte[] serverEncrypt = hkdfExpand(prk, "server encrypt".getBytes());
        byte[] clientEncrypt = hkdfExpand(serverEncrypt, "client encrypt".getBytes());
        byte[] serverMAC = hkdfExpand(clientEncrypt, "server MAC".getBytes());
        byte[] clientMAC = hkdfExpand(serverMAC, "client MAC".getBytes());
        byte[] serverIV = hkdfExpand(clientMAC, "server IV".getBytes());
        byte[] clientIV = hkdfExpand(serverIV, "client IV".getBytes());

        _serverEncrypt = new SecretKeySpec(serverEncrypt, _MacType);
        _clientEncrypt = new SecretKeySpec(clientEncrypt, _MacType);
        _serverMAC = new SecretKeySpec(serverMAC, _MacType);
        _clientMAC = new SecretKeySpec(clientMAC, _MacType);
        _serverIV = new IvParameterSpec(serverIV);
        _clientIV = new IvParameterSpec(serverEncrypt);
    }
}