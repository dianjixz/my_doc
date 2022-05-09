sudo mmdebstrap --architectures=riscv64 --include="debian-ports-archive-keyring" sid ./riscv64-chroot "deb http://deb.debian.org/debian-ports/ sid main" "deb http://deb.debian.org/debian-ports/ unreleased main"

sudo debootstrap --arch=riscv64 --keyring /usr/share/keyrings/debian-ports-archive-keyring.gpg --include=debian-ports-archive-keyring unstable ./riscv64-chroot http://deb.debian.org/debian-ports





$y$j9T$BtuMSzokF1q4MgNZ7k0gD0$C5sA8UOVq9rpcKaJcaon9R6CqnqmixJgvwD/LYdksl.
nihao




## 离线源使用指南

#!/bin/bash
echo "deb file:///mnt/mirror/ftp.ports.debian.org/debian-ports sid main" >> /etc/apt/sources.list

sudo apt -o Acquire::AllowInsecureRepositories=true -o Acquire::AllowDowngradeToInsecureRepositories=true update

apt -o APT::Get::AllowUnauthenticated=true install gnupg -y

cat /mnt/mirror/archive_2022.key | apt-key add -
apt update

apt remove vim-common
apt install vim


## riscv debian 源密钥

archive_2022.key


~~~ 

-----BEGIN PGP PUBLIC KEY BLOCK-----

mQINBF/7h7cBEAC9H4Seq0jkv3r3P+JNAoHi2O+X8Q3upG+JUnek3p8dNl+JyTEw
hVkDNttWhoqZOH+QLcL5oMd0Nr1zM9ng3pmarmX2DFk+aO95pnCoIlpA/nr0SQV/
fv9UQ15766ZeJriZAIz45TyrZ6VT3J6/mjPIqDNhB3Kso3ECbCafGVXRVtKxx97C
pmOxq0NgkPOk0WUfCAFLOyPgBcS4kNXFtnd/1sSuqz4KcfjLmQVGOKUl8m0kuXJ1
tqa2wBgvgtfeRSERLxGAJAxI7zGkpJejVCD8i5EigHwhnLZ2RIwDzyntbJh/MyXV
1een1/VfWF4tTDYLUqKcAOKdJ0efh9zF6hpY9jncrpIx6WXRvzOmZ0WzyVYmjJ/V
lXhxoLvEhg5tDfJgVwnkpHQKeD5aqWCbme2jiFUMf3EcRa31uKdMRZmKUM1IrpE6
HIuwCN/RmlDv//JQuuY8PUzhnJFtP/a42EdbIW/ScjGVolgPa15V37hNp32pZZzM
6ii4LiY/Kzow3Id6r9lprFl4J6P+wKfFH1dUFVRO5WeHe+ENDA8/2Q1DaC3ZN2yB
0kZLIV7pIpqGaT+CFSLfSuZS9lQd6+5qbjEqlWKhRHDtBnahKnISiCbg0zRWQ1vZ
xb4gFL2Es3vkbEY8/5AZoZ3EcVzKdUL3BVcBroCBSnqW+y6YH9IWZNkpPQARAQAB
tFVEZWJpYW4gUG9ydHMgQXJjaGl2ZSBBdXRvbWF0aWMgU2lnbmluZyBLZXkgKDIw
MjIpIDxmdHBtYXN0ZXJAcG9ydHMtbWFzdGVyLmRlYmlhbi5vcmc+iQJQBBMBCgA6
FiEEy8cKYLntbyN6X1sL6FJRT13zEvYFAl/7h7cCGwMFCQPeFoACCwkEFQoJCAUW
AgMBAAIeAQIXgAAKCRDoUlFPXfMS9gYIEACNuxw6YnhvMDL+l1ZtfH0fpY1ozAtz
yqxv4wzApsXS1A4GGPaaW0JcrEImTtdLO4kcOwEJi4Em6sAareaH2FeczQGfq66O
MffDhDI0UVw+TCs19N7Vf/Zh6io6zdrBK3bveHdhKvm5RSWZYvb5W1+epbYhJBig
McgIaV1Qvp+e8GV2u9nw7R8/QqHAip09z7WIJ/aauSfgQjbkX3pLdOszwTM+HP1z
SsOvBOQ6I4RQa8B+zpQuvf1TpwpR5zBhZbZQQW5uV4MyxpkRrpePIct1xjsai6is
SHnL8ITHzSyM7kGSnMDnGD97yYUOMgOaSYDXHkg3e/IYKmEP4OVVtXMG7ud9Y63G
zhOInykr8vyU5hpfL2Wg0w3VZh0lEiPYpRFx/leBoJp4nyeDp0eecj2424e6imEh
qHQEv5SZ8N+9frhYHNawHSTPvk1WJImgisKVYvmTFozGV1ucMgYje3sGkbOoU3wv
PMMEkyRibwLnUR8oHxIGouEcT+mOMmdzzCjHMzobr2v1M8+7BJw3UDLoTiI03uOX
xllJFHFCxU+0t0ImEW7Uk7rojIBluDZdRfwliI7fDCCnTFf1fF9e6gJUrKUNZFTp
yoUM8+FwlIhpEGhn4o8+ezyG83IebGVDxydnX1rrph+eZ9BvWWjesHo8R5FDvoPV
oPh9t/VGRvdQHA==
=KM28
-----END PGP PUBLIC KEY BLOCK-----

~~~
