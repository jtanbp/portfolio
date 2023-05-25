import java.io.*;
import java.math.BigInteger;
import java.net.Socket;
import java.security.*;
import java.security.cert.Certificate;
import java.security.spec.PKCS8EncodedKeySpec;
import javax.crypto.Mac;
import static java.lang.System.exit;

public class Client {
    final static String _Host = "127.0.0.1";
    final static int _Port = 8080;
    final static String _CASignedCert = "CASignedClientCertificate.pem";
    final static String _PrivateKey = "clientPrivateKey.der";
    final static String _SignType = "SHA256withRSA";
    final static String _MacType = "HmacSHA256";
    public static void main(String[] args) throws Exception {

        Socket socketToServer = new Socket(_Host, _Port);

        ObjectOutputStream out = new ObjectOutputStream( socketToServer.getOutputStream());
        ObjectInputStream in = new ObjectInputStream(socketToServer.getInputStream());
        System.out.println("Client Connected");

        // send Nonce1 to server
        byte Nonce[] = SecureRandom.getSeed(32);
        out.writeObject(Nonce);

        // Certificate
        Certificate certificate = Handshake.getCertificate(_CASignedCert);

        // Using DH method to compute Key
        byte DHPrivateKeyArr[] = SecureRandom.getSeed(32);
        BigInteger DHPrivateKey = new BigInteger(DHPrivateKeyArr);
        BigInteger DHPublicKey = Handshake.g.modPow(DHPrivateKey, Handshake.n);

        KeyFactory keyFactory = KeyFactory.getInstance("RSA");

        // Generate PrivateKey
        File PrivateKeyFile = new File(_PrivateKey);
        FileInputStream PrivateKeyFileStream = new FileInputStream(PrivateKeyFile);
        byte PrivateKeyString[] = PrivateKeyFileStream.readAllBytes();

        PKCS8EncodedKeySpec KeySpec = new PKCS8EncodedKeySpec(PrivateKeyString);
        PrivateKey PrivateKey = keyFactory.generatePrivate(KeySpec);

        // Signing Key
        Signature PrivateSignature = Signature.getInstance(_SignType);
        PrivateSignature.initSign(PrivateKey);
        PrivateSignature.update(DHPublicKey.toByteArray());
        byte[] DHSignedPublicKey = PrivateSignature.sign();

        Certificate serverCert = (Certificate) in.readObject();
        BigInteger serverDHPublicKey = (BigInteger) in.readObject();
        byte[] serverDHSignedPublicKey = (byte[]) in.readObject();


        out.writeObject(certificate);
        out.writeObject(DHPublicKey);
        out.writeObject(DHSignedPublicKey);
        out.flush();

        // calculate shared secret key
        BigInteger SharedKey = serverDHPublicKey.modPow((BigInteger) DHPrivateKey, Handshake.n);

        // get secret keys
        Handshake.makeSecretKeys(Nonce, SharedKey.toByteArray());


        // mac encrypt all handshake messages so far for comparing with server records
        Mac mac = Mac.getInstance(_MacType);
        mac.init(Handshake._serverMAC);

        // Mac encrypt all previous messages
        byte[] NonceMac = mac.doFinal(Nonce);

        byte[] serverCertMac = mac.doFinal(serverCert.getEncoded());
        byte[] serverDHPublicKeyMac = mac.doFinal(serverDHPublicKey.toByteArray());
        byte[] serverDHSignedPublicKeyMac = mac.doFinal(serverDHSignedPublicKey);

        byte[] clientCertMac = mac.doFinal(certificate.getEncoded());
        byte[] clientDHPublicKeyMac = mac.doFinal(DHPublicKey.toByteArray());
        byte[] clientDHSignedPublicKeyMac = mac.doFinal(DHSignedPublicKey);

        // Write to Byte Stream
        ByteArrayOutputStream clientHandshake = new ByteArrayOutputStream();
        clientHandshake.writeBytes(NonceMac);
        clientHandshake.writeBytes(serverCertMac);
        clientHandshake.writeBytes(serverDHPublicKeyMac);
        clientHandshake.writeBytes(serverDHSignedPublicKeyMac);
        clientHandshake.writeBytes(clientCertMac);
        clientHandshake.writeBytes(clientDHPublicKeyMac);
        clientHandshake.writeBytes(clientDHSignedPublicKeyMac);

        byte[] serverHandshakeByteArr = (byte[]) in.readObject();
        byte[] clientHandshakeByteArr = clientHandshake.toByteArray();

        for(int i = 0; i < clientHandshakeByteArr.length; i++){
            if(serverHandshakeByteArr[i]!=clientHandshakeByteArr[i]){
                System.out.println("Error: Handshake Records do not match!");
                exit(1);
            }
        }
        System.out.println("Handshake Success!");


        // mac encrypt all handshake messages so far with clientMac for sending to server
        mac = Mac.getInstance(_MacType);
        mac.init(Handshake._clientMAC);

        // Mac Encryption
        NonceMac = mac.doFinal(Nonce);

        serverCertMac = mac.doFinal(serverCert.getEncoded());
        serverDHPublicKeyMac = mac.doFinal(serverDHPublicKey.toByteArray());
        serverDHSignedPublicKeyMac = mac.doFinal(serverDHSignedPublicKey);

        clientCertMac = mac.doFinal(certificate.getEncoded());
        clientDHPublicKeyMac = mac.doFinal(DHPublicKey.toByteArray());
        clientDHSignedPublicKeyMac = mac.doFinal(DHSignedPublicKey);

        // Write to Byte Stream
        clientHandshake = new ByteArrayOutputStream();
        clientHandshake.writeBytes(NonceMac);
        clientHandshake.writeBytes(serverCertMac);
        clientHandshake.writeBytes(serverDHPublicKeyMac);
        clientHandshake.writeBytes(serverDHSignedPublicKeyMac);
        clientHandshake.writeBytes(clientCertMac);
        clientHandshake.writeBytes(clientDHPublicKeyMac);
        clientHandshake.writeBytes(clientDHSignedPublicKeyMac);

        // Write byte stream to output
        out.writeObject(clientHandshake.toByteArray());


        // Receiving explicitly 2 message from Server
        byte[] messageFromServer = (byte[]) in.readObject();
        byte[] messageFromServer2 = (byte[]) in.readObject();

        byte[] stringFromServer = Message.decrypt(messageFromServer, Handshake._serverIV.getIV(), Handshake._serverEncrypt.getEncoded());
        byte[] stringFromServer2 = Message.decrypt(messageFromServer2, Handshake._serverIV.getIV(), Handshake._serverEncrypt.getEncoded());

        for(int i = 0; i < stringFromServer.length; i++){
            System.out.print((char) stringFromServer[i]);
        }
        System.out.println();
        for(int i = 0; i < stringFromServer2.length; i++){
            System.out.print((char) stringFromServer2[i]);
        }
        System.out.println();

        // Sending Message to Server
        String message = "1st Message to Server";
        out.writeObject(Message.encrypt(Handshake._clientMAC.getEncoded(), message.getBytes(), Handshake._clientIV.getIV(), Handshake._clientEncrypt.getEncoded()));
    }
}