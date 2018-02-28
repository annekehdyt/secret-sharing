# secret-sharing
This program based on the 1979 Shamir's secret sharing, which is the first secret sharing scheme ever published. You can find it here : <br>
https://dl.acm.org/citation.cfm?id=359176 <br>

Supposed we have one bit of secret message that wanted to be encrypted, and wanted to share the keys with a group of n participants. The condition is that it is possible to decrypt the message with only t participants, where 1<=t<=n. 

In this program, we modified the computation under finite field -> Galois Field (2^8), instead of standard arithmetic calculation. 
