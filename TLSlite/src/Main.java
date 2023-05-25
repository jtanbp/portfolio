import java.io.*;
import java.math.BigInteger;
import java.security.*;
import java.security.cert.Certificate;
import java.security.cert.CertificateException;
import java.security.cert.CertificateFactory;
import java.security.spec.InvalidKeySpecException;
import java.security.spec.PKCS8EncodedKeySpec;

public class Main {
    final static String _CAServerSignedCert = "CASignedServerCertificate.pem";
    final static String _ServerPrivateKey = "serverPrivateKey.der";
    final static String _CAClientSignedCert = "CASignedClientCertificate.pem";
    final static String _ClientPrivateKey = "clientPrivateKey.der";
    final static String _SignType = "SHA256withRSA";
    final static String _Certificate = "CAcertificate.pem";
    final static String _publicKeyString = "FFFFFFFFFFFFFFFFC90FDAA22168C234C4C6628B" +
            "80DC1CD129024E088A67CC74020BBEA63B139B22514A08798E3404DDEF9" +
            "519B3CD3A431B302B0A6DF25F14374FE1356D6D51C245E485B576625E7E" +
            "C6F44C42E9A637ED6B0BFF5CB6F406B7EDEE386BFB5A899FA5AE9F24117C" +
            "4B1FE649286651ECE45B3DC2007CB8A163BF0598DA48361C55D39A69163FA" +
            "8FD24CF5F83655D23DCA3AD961C62F356208552BB9ED529077096966D670C" +
            "354E4ABC9804F1746C08CA18217C32905E462E36CE3BE39E772C180E86039" +
            "B2783A2EC07A28FB5C55DF06F4C52C9DE2BCBF6955817183995497CEA956A" +
            "E515D2261898FA051015728E5A8AACAA68FFFFFFFFFFFFFFFF";
    public static void main(String[] args) throws IOException, CertificateException, NoSuchAlgorithmException, InvalidKeySpecException, InvalidKeyException, SignatureException, NoSuchProviderException {
        BigInteger g = new BigInteger("2");
        BigInteger n = new BigInteger(_publicKeyString, 16);

        // Server Code

        // Server Certificate
        File serverCertificateFile = new File (_CAServerSignedCert);
        FileInputStream serverCertificateInputStream = new FileInputStream(serverCertificateFile);
        CertificateFactory serverCertificateFactory = CertificateFactory.getInstance("X.509");
        Certificate serverCertificate = serverCertificateFactory.generateCertificate(serverCertificateInputStream);

        // Generating DH
        byte DHserverPrivateKeyArr[] = SecureRandom.getSeed(32);
        BigInteger DHserverPrivateKey = new BigInteger(DHserverPrivateKeyArr);
        BigInteger DHserverPublicKey = g.modPow(DHserverPrivateKey, n);

        KeyFactory keyFactory = KeyFactory.getInstance("RSA");

        File serverPrivateKeyFile = new File (_ServerPrivateKey);
        FileInputStream serverPrivateKeyFileStream = new FileInputStream(serverPrivateKeyFile);
        byte serverPrivateKeyString[] = serverPrivateKeyFileStream.readAllBytes();

        PKCS8EncodedKeySpec serverKeySpec = new PKCS8EncodedKeySpec(serverPrivateKeyString);
        PrivateKey serverPrivateKey = keyFactory.generatePrivate(serverKeySpec);

        // Signing Certificate
        Signature serverPrivateSignature = Signature.getInstance(_SignType);
        serverPrivateSignature.initSign(serverPrivateKey);
        serverPrivateSignature.update(DHserverPublicKey.toByteArray());

        byte[] serverSignedPublicKey = serverPrivateSignature.sign();
        // End of Server Code

        // Client Section

        File clientCertificateFile = new File (_CAClientSignedCert);
        FileInputStream clientCertificateInputStream = new FileInputStream(clientCertificateFile);
        CertificateFactory clientCertificateFactory = CertificateFactory.getInstance("X.509");
        Certificate clientCertificate = serverCertificateFactory.generateCertificate(clientCertificateInputStream);

        byte DHclientPrivateKeyArr[] = SecureRandom.getSeed(32);
        BigInteger DHclientPrivateKey = new BigInteger(DHclientPrivateKeyArr);


        BigInteger DHclientPublicKey = g.modPow(DHclientPrivateKey, n);

        // Make Private Key
        File clientPrivateKeyFile = new File (_ClientPrivateKey);
        FileInputStream clientPrivateKeyFileStream = new FileInputStream(clientPrivateKeyFile);
        byte clientPrivateKeyString[] = clientPrivateKeyFileStream.readAllBytes();

        PKCS8EncodedKeySpec clientKeySpec = new PKCS8EncodedKeySpec(clientPrivateKeyString);
        PrivateKey clientPrivateKey = keyFactory.generatePrivate(clientKeySpec);

        // Signing
        Signature clientPrivateSignature = Signature.getInstance(_SignType);
        clientPrivateSignature.initSign(clientPrivateKey);
        clientPrivateSignature.update(DHclientPublicKey.toByteArray());

        byte[] clientSignedPublicKey = clientPrivateSignature.sign();
        // End of Client Code

        // Verification Section

        Certificate CA = Handshake.getCertificate(_Certificate);
        PublicKey CApublicKey = CA.getPublicKey();
        clientCertificate.verify(CApublicKey);

        Signature serverVerifySigs = Signature.getInstance(_SignType);
        byte[] DHserverPublic = DHserverPublicKey.toByteArray();
        serverVerifySigs.initVerify(serverCertificate.getPublicKey());
        serverVerifySigs.update(DHserverPublic);
        System.out.println("Server Verification = " + serverVerifySigs.verify(serverSignedPublicKey));

        Signature clientVerifySigs = Signature.getInstance(_SignType);
        byte[] DHclientPublic = DHclientPublicKey.toByteArray();
        clientVerifySigs.initVerify(clientCertificate.getPublicKey());
        clientVerifySigs.update(DHclientPublic);
        System.out.println("Client Verification = " + clientVerifySigs.verify(clientSignedPublicKey));
    }
}