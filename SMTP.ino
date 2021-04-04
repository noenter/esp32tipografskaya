void SMTP_Init(){
  smtpData.setLogin(smtpServer, smtpServerPort, emailSenderAccount, emailSenderPassword);
  smtpData.setSender("ESP32", emailSenderAccount);
  smtpData.setPriority("High");
  smtpData.setSubject(emailSubject);
  smtpData.setMessage("Hello World! - ESP Online", false);
  smtpData.addRecipient(emailRecipient);
  smtpData.setSendCallback(sendCallback);
if (!MailClient.sendMail(smtpData))
    Serial.println("Error sending Email, " + MailClient.smtpErrorReason());
//Clear all data from Email object to free memory
//  smtpData.empty();
}

void sendCallback(SendStatus msg) {
  // Print the current status
  Serial.println(msg.info());

  // Do something when complete
  if (msg.success()) {
    Serial.println("----------------");
  }
}

void sendCallback(SendStatus info);
