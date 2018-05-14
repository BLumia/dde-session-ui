<a name="4.3.7.3"></a>
## 4.3.7.3 (2018-05-14)


#### Features

*   disable login animation for mips ([7a1e9827](https://github.com/linuxdeepin/dde-session-ui/commit/7a1e9827fa84404b4e7f2d6a8042dd90481f8a9f))

#### Bug Fixes

*   remove pbis-open for sw and mips ([1dcd34f2](https://github.com/linuxdeepin/dde-session-ui/commit/1dcd34f25391c9bed604cd64191461732a1d30f6))
* **wm-chooser:**  json type error ([45a4c063](https://github.com/linuxdeepin/dde-session-ui/commit/45a4c0637b5f06b994058bb884251fc1b8d31282))



<a name="4.3.7.2"></a>
## 4.3.7.2 (2018-04-19)


#### Bug Fixes

*   force kill application ([5b9450e4](https://github.com/linuxdeepin/dde-session-ui/commit/5b9450e4dfbc2ff9e210d3b147d41db551f4351e))
*   not avatar when disable animation ([77c0e17d](https://github.com/linuxdeepin/dde-session-ui/commit/77c0e17debdd6afe97a73adcaa2dfdec4636654b))
* **dmemory:**  regular expression capture error ([40ed51f9](https://github.com/linuxdeepin/dde-session-ui/commit/40ed51f9a752b8b803abd20ef9849fed350b1167))
* **shutdown:**  switch user button will show when prompt interface ([5976f883](https://github.com/linuxdeepin/dde-session-ui/commit/5976f8830b91faa706bd32f5b85e0115cc6338c3))



<a name=""></a>
##  4.3.7.1 (2018-04-11)


#### Features

*   Limit proc min memory usage is display to 10MB ([a647b6cf](https://github.com/linuxdeepin/dde-session-ui/commit/a647b6cf05eccf6de79d9798da1bd951e78187a4))

#### Bug Fixes

*   Compile error because of macro ([01f4e686](https://github.com/linuxdeepin/dde-session-ui/commit/01f4e686a403a357453bd4f8a66b975e4daa7341))
* **memory-warning-dialog:**  deepin-wm is in the free list ([76b44595](https://github.com/linuxdeepin/dde-session-ui/commit/76b445959a724b1c07648e159c46c8ab54bc28cd))



<a name="4.3.7"></a>
### 4.3.7 (2018-03-29)


#### Bug Fixes

* **greeter:**  not check user state ([543c79b6](https://github.com/linuxdeepin/dde-session-ui/commit/543c79b618c0fb5e5b2d61e5ee88791b8f2fc906))



<a name="4.3.6"></a>
### 4.3.6 (2018-03-28)


#### Bug Fixes

*   AD user login prompt error ([4f7687c8](https://github.com/linuxdeepin/dde-session-ui/commit/4f7687c8c7bc85c90a91bd60ab3c4d6c771d75d1))



<a name="4.3.5"></a>
### 4.3.5 (2018-03-27)




<a name="4.3.4"></a>
### 4.3.4 (2018-03-27)


#### Bug Fixes

*   remove the blank at the begging and end of the message ([9f0a84a7](https://github.com/linuxdeepin/dde-session-ui/commit/9f0a84a731499343116abeea3ee5a4f2c179ba26))
* **greeter:**  esc abort operation not restore widget visible ([f8c7b79d](https://github.com/linuxdeepin/dde-session-ui/commit/f8c7b79d041b9d22693e4003f55bcb2c9aece3b8))



<a name="4.3.3"></a>
### 4.3.3 (2018-03-26)


#### Bug Fixes

* **greeter:**  not need check current user ([56c13603](https://github.com/linuxdeepin/dde-session-ui/commit/56c13603848fae82a2fc162d98d6f0d788e904f2))



<a name="4.3.2"></a>
### 4.3.2 (2018-03-22)


#### Features

*   If the last user was not found, initialize the AD login button ([dd5cf0be](https://github.com/linuxdeepin/dde-session-ui/commit/dd5cf0be55638da8d4732b33faf96d56f88e3aa2))
*   keep user list order ([589c58e1](https://github.com/linuxdeepin/dde-session-ui/commit/589c58e1554f3a9f1fd6f2097f9dcc2629fd377e))

#### Bug Fixes

*   alert tip not hide ([3efa4cf7](https://github.com/linuxdeepin/dde-session-ui/commit/3efa4cf74f56dfa23ff44818e85369b570d023ae))
*   not emit user list changed when append user ([3f70d9a8](https://github.com/linuxdeepin/dde-session-ui/commit/3f70d9a83945721b62c9dd793c3e0a9de16dc317))
* **fullscreen:**  Handle multi-screen geometry correctly ([a2dede2b](https://github.com/linuxdeepin/dde-session-ui/commit/a2dede2b1418964a00a521786ae2061076d3a83a))
* **greeter:**
  *  not select user kblayout ([6b04a65d](https://github.com/linuxdeepin/dde-session-ui/commit/6b04a65dae86bbf59fa8617ec160f34e3612b9e9))
  *  not set greeter authenticate user ([a50c9a78](https://github.com/linuxdeepin/dde-session-ui/commit/a50c9a78edc4f69df681e843f30cb523eb941f62))
  *  Prevent duplicate settings of current user ([c1ab1484](https://github.com/linuxdeepin/dde-session-ui/commit/c1ab1484818011ab76bbe383101b25d33e923ef4))
* **osd:**  not fillet after startup ([aec60e2e](https://github.com/linuxdeepin/dde-session-ui/commit/aec60e2e7e9970ad5af204cd4f945db86465dda3))
* **welcome:**
  *  restore hotzone with welcome quit ([307f1de4](https://github.com/linuxdeepin/dde-session-ui/commit/307f1de41ce2007e39f222849d92f964ac857735))
  *  error dbus call ([d21d766e](https://github.com/linuxdeepin/dde-session-ui/commit/d21d766ebe561d3619d8f7bb244e9e8728f9bd56))



<a name="4.3.1"></a>
### 4.3.1 (2018-03-16)


#### Bug Fixes

*   dde warning dialog not move to center ([f8753511](https://github.com/linuxdeepin/dde-session-ui/commit/f8753511fcecd2b972a8e56a2b10f3849deccd47))
*   check cursor position error when screens scale ([94b4e4c1](https://github.com/linuxdeepin/dde-session-ui/commit/94b4e4c1a0fcb5b6ed6cc9773aa5fc79993d2c15))
*   black screen when not have blur image ([1b049645](https://github.com/linuxdeepin/dde-session-ui/commit/1b049645e2833270097df3b8fa369c28f38ec237))
* **greeter:**  refresh the AD state after staring ([09bd9d69](https://github.com/linuxdeepin/dde-session-ui/commit/09bd9d69e869bcb1236d5c4ad0dfbd6572637072))
* **lock:**  error return greeter wallpaper ([f34bf490](https://github.com/linuxdeepin/dde-session-ui/commit/f34bf490bc9465a94d53c3e48398f05aeea45d5d))
* **welcome:**  not disable zone ([0cacd0a9](https://github.com/linuxdeepin/dde-session-ui/commit/0cacd0a979c19554b432c72003bf2f4f4fd54083))
* **widgets:**  user icon not align center and icon size error ([0c2b4f64](https://github.com/linuxdeepin/dde-session-ui/commit/0c2b4f6482cae797c753d54b67ee5b8609f63ec5))



<a name=""></a>
##  4.3.0 (2018-03-14)


#### Features

*   remove dman portal, use deepin-manual. ([3bd028cf](https://github.com/linuxdeepin/dde-session-ui/commit/3bd028cf135ad22c784c0146e447ef34a69af768))
*   add runtime dep ([f4a86d1b](https://github.com/linuxdeepin/dde-session-ui/commit/f4a86d1b9cf82073b0492a67d70a5bd2a7523d7b))
* **dde-welcome:**  Set the mouse style to wait ([b421fc33](https://github.com/linuxdeepin/dde-session-ui/commit/b421fc33a25f7e111deb722cc4c2f0993d0add6b))
* **greeter:**
  *  save last user ([7ffb768e](https://github.com/linuxdeepin/dde-session-ui/commit/7ffb768ef0654d021e5e6fe73c9159584552a64e))
  *  support other login ([1466f7fd](https://github.com/linuxdeepin/dde-session-ui/commit/1466f7fd95636254b09b1791a44980048fa7f3fe))
  *  support AD user login ([c70c613a](https://github.com/linuxdeepin/dde-session-ui/commit/c70c613ab1ddc683de09f945dfdfa2a523b9c077))
* **lock:**
  *  support switch to other ([e8a8b438](https://github.com/linuxdeepin/dde-session-ui/commit/e8a8b438172b391e6c16a82ab9ce07f4e5ba0685))
  *  support other login ([8c2d7277](https://github.com/linuxdeepin/dde-session-ui/commit/8c2d72770cd9bb8b5a3948e7d71b485e43550404))
* **osd:**  use new dock indicator plugin config ([f34edcc7](https://github.com/linuxdeepin/dde-session-ui/commit/f34edcc76eb7a2261b5189fdd319f28a07922a75))
* **otheruserinput:**
  *  add alert tip ([d37d7401](https://github.com/linuxdeepin/dde-session-ui/commit/d37d74014ad7b2a7cefe88d5526043c609c8aadf))
  *  add new widget ([eda373ab](https://github.com/linuxdeepin/dde-session-ui/commit/eda373abffb689acd457e55ecdafba6d43d79559))
* **userwidget:**  use signal update button ([bc275b31](https://github.com/linuxdeepin/dde-session-ui/commit/bc275b31c2b920671484949e8f585b5ecbadb971))
* **welcome:**  support login animation ([8d614a71](https://github.com/linuxdeepin/dde-session-ui/commit/8d614a7189dfe7eb63300cb3a4be4fb62c7db48b))

#### Bug Fixes

*   switch to user not grab keyboard ([2b594f1b](https://github.com/linuxdeepin/dde-session-ui/commit/2b594f1b5a67dbed70a657b3de34ac7800d5dd92))
*   click user icon will refresh password edit ([d19f5831](https://github.com/linuxdeepin/dde-session-ui/commit/d19f583111cbbea557a7adbd5a83bb64018c9ff2))
*   using new dbus interface ([2df0514e](https://github.com/linuxdeepin/dde-session-ui/commit/2df0514e882c1263e57d73bd960a72d0404f0a30))
*   widgets position error ([18b2d19e](https://github.com/linuxdeepin/dde-session-ui/commit/18b2d19e9182a96cb5a7e1644f1231924a45bf2b))
*   session cannot choose one session of enter key ([55e8eda0](https://github.com/linuxdeepin/dde-session-ui/commit/55e8eda0905aee6315d76133fd04a54ebcf642c3))
*   not update user widget when choosed session ([262b020f](https://github.com/linuxdeepin/dde-session-ui/commit/262b020f7c6a46711e2eb7235a2708607bc6a99e))
*   fullscreen background geometry error ([d5fa74c6](https://github.com/linuxdeepin/dde-session-ui/commit/d5fa74c6d23afd119b4ec787a4a2b751385a7ab0))
*   set focus when program startup ([6fe04c6a](https://github.com/linuxdeepin/dde-session-ui/commit/6fe04c6a2f8781e581c0088ef069bd933742ac87))
*   Adapt lintian ([b32cdb0c](https://github.com/linuxdeepin/dde-session-ui/commit/b32cdb0c3538b3c285d79d4409012430e100f442))
* **UserWidget:**  update buttons position ([11c5eb1d](https://github.com/linuxdeepin/dde-session-ui/commit/11c5eb1d2e763d06cacb60295f550f03baeab68a))
* **background:**  set default wallpaper when file not exist ([ec513677](https://github.com/linuxdeepin/dde-session-ui/commit/ec5136778cd497b1ad63356872e2accf4e3ad156))
* **dde-welcome:**  not save log to .cache/deepin ([430e99d4](https://github.com/linuxdeepin/dde-session-ui/commit/430e99d4be4bbfa6d8a809a8167e6f1403eea8ab))
* **greeter:**
  *  widgets position error ([2d829efc](https://github.com/linuxdeepin/dde-session-ui/commit/2d829efcabc65e4f301d381d481e026efc4fe093))
  *  widgets position error ([18904382](https://github.com/linuxdeepin/dde-session-ui/commit/18904382e6c38446a0ba223c843c937e646d43bb))
  *  not hide userwidget when choose session and power ([e15195c2](https://github.com/linuxdeepin/dde-session-ui/commit/e15195c29683e370a60940f2d486b4fb9cdb016d))
  *  not auth user ([9ffe43c8](https://github.com/linuxdeepin/dde-session-ui/commit/9ffe43c89d8a9f6c96ae3aaa502957f00e0dc82e))
  *  nopasswd user cannot login ([a6fd7ab4](https://github.com/linuxdeepin/dde-session-ui/commit/a6fd7ab4fcbde865b6b841afbd2577e1dea75faf))
  *  add timeout ([91bf150d](https://github.com/linuxdeepin/dde-session-ui/commit/91bf150d19fdaca93546e7c8b8d6b73107869d42))
  *  crash when can't find session name ([9efab64e](https://github.com/linuxdeepin/dde-session-ui/commit/9efab64e69ebba804c0ab84d25bc77b04cca9648))
  *  use new background dbus ([a5157308](https://github.com/linuxdeepin/dde-session-ui/commit/a515730865b5b7b99251e1579249815b74a8e505))
  *  cursor not apply scale ([943ae919](https://github.com/linuxdeepin/dde-session-ui/commit/943ae9192b92acd78bee3145972fad639d520d4d))
  *  remove save last session ([dcf12214](https://github.com/linuxdeepin/dde-session-ui/commit/dcf12214519001b9ed79005016a898845becf9a0))
* **layout:**  fix user name layout ([3d68229c](https://github.com/linuxdeepin/dde-session-ui/commit/3d68229ca0da14061fc8cd775edadc8ba23cd672))
* **lock:**
  *  not grab keyboard ([3b20c2c3](https://github.com/linuxdeepin/dde-session-ui/commit/3b20c2c3a0dfb9f2ccb907d6274a14c48a017be0))
  *  canel shutdown not hide warning tip ([98841fc0](https://github.com/linuxdeepin/dde-session-ui/commit/98841fc096c74aae57261a520df930133733eedb))
  *  control center not hide ([c465e367](https://github.com/linuxdeepin/dde-session-ui/commit/c465e367aec8b80b9a1ebbb8346ae5fa0a883d51))
  *  No focus after staring ([8ef1f837](https://github.com/linuxdeepin/dde-session-ui/commit/8ef1f837c578653e86626c9392db0bd88387cc20))
  *  volume number color is black ([575a8a48](https://github.com/linuxdeepin/dde-session-ui/commit/575a8a48dc8b96ead2cf0b3a714f005eb75e1ca2))
  *  mpris widget not set style ([a587d21a](https://github.com/linuxdeepin/dde-session-ui/commit/a587d21a0cc3aea22565edfbdb82ad2a286061cd))
* **shutdown:**
  *  not check wallpaper is local file ([57839e13](https://github.com/linuxdeepin/dde-session-ui/commit/57839e139da7617721b3b5f371795b78e36161d8))
  *  remove multi user warning label's fixed width ([223e9fac](https://github.com/linuxdeepin/dde-session-ui/commit/223e9fac5427244165ea4262c5bfa25d4b17134f))
* **welcome:**
  *  update success svg not support HiDPI ([7cbe1b22](https://github.com/linuxdeepin/dde-session-ui/commit/7cbe1b2285575fcd04252e056f81d01fdb80cc4c))
  *  Set the wrong window flag ([05458296](https://github.com/linuxdeepin/dde-session-ui/commit/05458296ff020a2a81c40ac78e059b4c3ed41ced))
  *  fit multi screen ([09c211cb](https://github.com/linuxdeepin/dde-session-ui/commit/09c211cb5cfc26a87ce2da401a0f5cc656eb73c6))
* **widgets:**
  *  use compatibility interface with user background ([6375fde4](https://github.com/linuxdeepin/dde-session-ui/commit/6375fde445ea1bd57ba152ba647e7e0a9949232c))
  *  AD user cannot login when userwidget not releasekeyboard ([5c1043ea](https://github.com/linuxdeepin/dde-session-ui/commit/5c1043ead4626d3933aed2d74e062ef7cf7adef4))
  *  setting properties for many times ([c3ba458b](https://github.com/linuxdeepin/dde-session-ui/commit/c3ba458bd45bdb7d0c107f55ff91e616318de0d5))
  *  other user input set focus type ([03912aa5](https://github.com/linuxdeepin/dde-session-ui/commit/03912aa5ddf105297d6095261ebf9c42f7e18ffe))



<a name="4.1.7"></a>
### 4.1.7 (2017-11-28)


#### Bug Fixes

* **greeter:**
  *  unable to login other desktop when user is nopassword ([4827e09b](https://github.com/linuxdeepin/dde-session-ui/commit/4827e09b1f3b7e9ab3efdfd9ab4ab5f99892c84a))
  *  error auth password when nopassword user ([e3986aa8](https://github.com/linuxdeepin/dde-session-ui/commit/e3986aa8123410b84c49dbf2b6ebd90d9f0225c8))



<a name="4.1.6"></a>
### 4.1.6 (2017-11-23)


#### Bug Fixes

* **lock:**  don't show alert when user is nopasswdlogin ([97f52db0](https://github.com/linuxdeepin/dde-session-ui/commit/97f52db085b285030cdc381c9c7782f72a5cdf7b))
* **shutdown:**  cannot open system monitor ([7e3156fa](https://github.com/linuxdeepin/dde-session-ui/commit/7e3156fa6e35d1710938e5af0fcb2b9678c20b09))



<a name="4.1.5"></a>
### 4.1.5 (2017-11-20)


#### Features

* **osd:**  change hide time to 1 second ([615d7208](https://github.com/linuxdeepin/dde-session-ui/commit/615d7208c864c84454c4dbe1e2e449e45f0fae5a))



<a name="4.1.4"></a>
### 4.1.4 (2017-11-16)


#### Bug Fixes

* **shutdown:**  background white ([d1f8809c](https://github.com/linuxdeepin/dde-session-ui/commit/d1f8809c0ed85956ce79c963f0ddf1cdf64baff2))



<a name="4.1.3"></a>
### 4.1.3 (2017-11-16)


#### Bug Fixes

* **background:**  refresh wallpaper when not the same file ([43931726](https://github.com/linuxdeepin/dde-session-ui/commit/43931726241f5e42a8f7b07cc88e435a11a30e07))
* **dde-lock:**  some widget shown too early ([fa857876](https://github.com/linuxdeepin/dde-session-ui/commit/fa85787610cc18765bf19cec0230171b2554a332))
* **greeter:**  skip when click self ([3128a549](https://github.com/linuxdeepin/dde-session-ui/commit/3128a54987df85c6c9e3407d1be2c55008506cc1))
* **lock:**  userwidget pos error when size changed ([13775f5b](https://github.com/linuxdeepin/dde-session-ui/commit/13775f5b66f0faea0dd58ed3a11e688e69214373))

#### Features

*   add dde-dman-portal ([f7772551](https://github.com/linuxdeepin/dde-session-ui/commit/f77725511b03162073699ca8c67ee9292f6d8363))
* **shutdown:**  support display user displayname ([c7a3fe69](https://github.com/linuxdeepin/dde-session-ui/commit/c7a3fe69780e1f9c209e5ea5044fabc43d948545))
* **user:**  support display user fullname ([34f2f9ff](https://github.com/linuxdeepin/dde-session-ui/commit/34f2f9ffc48819ec46d4b62f76e48452cf4b8d0e))
* **userwidget:**  login user interface updated ([7eeeb589](https://github.com/linuxdeepin/dde-session-ui/commit/7eeeb58991a7fbe76e391a9fd046c57f3c35a635))



<a name="4.1.2"></a>
### 4.1.2 (2017-11-10)


#### Bug Fixes

* **greeter:**  nopasswordlogin function failure ([c02c4d3c](https://github.com/linuxdeepin/dde-session-ui/commit/c02c4d3cd9eb6c7f4a2a337542ed8eee436b0a75))
* **lock:**  use dgettext update ts with fprint ([43ba4a3e](https://github.com/linuxdeepin/dde-session-ui/commit/43ba4a3e6369e7db57bba1f04df94c8690d27bbb))
* **shutdown:**  set default background when get blur error ([9f7ab4e2](https://github.com/linuxdeepin/dde-session-ui/commit/9f7ab4e2caa567275e892477e6667feeac46828f))



<a name="4.1.1"></a>
### 4.1.1 (2017-11-09)


#### Bug Fixes

*   osd support multiple screens list ([5a8b880c](https://github.com/linuxdeepin/dde-session-ui/commit/5a8b880cc7988c4774f59bab1888304b128f2ca7))
* **greeter:**  switch user will login ([4cb700d7](https://github.com/linuxdeepin/dde-session-ui/commit/4cb700d75cb9fae8ec30936b97bffdf68338110a))
* **hidpi:**  window placed at wrong position ([8252e1ee](https://github.com/linuxdeepin/dde-session-ui/commit/8252e1ee5f2015cec76f28430a64fc0f8fba665b))
* **lock:**  update ts with fprint ([acfa282c](https://github.com/linuxdeepin/dde-session-ui/commit/acfa282c362a96ec4788de34d279ac9951fcbf7e))
* **osd:**
  *  support dapplication single instance ([d04ef2e8](https://github.com/linuxdeepin/dde-session-ui/commit/d04ef2e858a5c9744e14d93d244c993cafe64a25))
  *  scroll to current item when direct switch ([766a1116](https://github.com/linuxdeepin/dde-session-ui/commit/766a11161f9b2bcd9530f436427c70771f47dae7))

#### Features

* **background:**  use blur image dbus ([2f135276](https://github.com/linuxdeepin/dde-session-ui/commit/2f135276126bd9ad48a6ad446a48fc3190d98930))
* **dde-welcome:**  adding detection rules ([426bc903](https://github.com/linuxdeepin/dde-session-ui/commit/426bc903ac40093cbc5961f1843baa81fbb17e1e))



<a name=""></a>
##  4.1.0 (2017-11-03)


#### Features

* **dde-warning-dialog:**  dbus interface ([4bd30af4](https://github.com/linuxdeepin/dde-session-ui/commit/4bd30af46ad76d09257ed319a0f3f68945945430))
* **shutdown:**
  *  use dapplication and set default theme ([1546dd20](https://github.com/linuxdeepin/dde-session-ui/commit/1546dd2007a092e66141301fca3957ac3457d7d9))
  *  apply system font size settings ([65b8a11f](https://github.com/linuxdeepin/dde-session-ui/commit/65b8a11f9b16e53c9a264239f2d8624e70cb468f))

#### Bug Fixes

* **lock:**
  *  not show error message when finger verification ([e831d6bd](https://github.com/linuxdeepin/dde-session-ui/commit/e831d6bd8c1debbbbfca4b53b033593a8c733975))
  *  show wrong passwd premature ([0a56de2e](https://github.com/linuxdeepin/dde-session-ui/commit/0a56de2e6d259ab4bfd2233ea56c9e81043d15aa))
* **osd:**  cannot scroll when error index ([6f307bc1](https://github.com/linuxdeepin/dde-session-ui/commit/6f307bc1584199d7ff2824ba6d9e347285c27b45))
* **shutdown:**
  *  not grab keyboard ([aac6cf88](https://github.com/linuxdeepin/dde-session-ui/commit/aac6cf88ed84a49c5a3f398f35ed5383eee2f5d5))
  *  white screen when not use deepin wm ([9a5c78db](https://github.com/linuxdeepin/dde-session-ui/commit/9a5c78db5d0c4e3f6baa79266eb10258f9f84078))
  *  not disable hotzone when show ([539c8b99](https://github.com/linuxdeepin/dde-session-ui/commit/539c8b997f735fd5ff794463d2554c945002b1e1))



<a name=""></a>
##  4.0.17 (2017-10-26)


#### Bug Fixes

*   svg file error ([684de9e7](https://github.com/linuxdeepin/dde-session-ui/commit/684de9e76cd66eef384045d5671c3a5dbe907177))
*   correct header filename ([7cc8316b](https://github.com/linuxdeepin/dde-session-ui/commit/7cc8316bf62617e4e6fb0093e8fa5fa6f30c14fb))
*   upgrade success not load ts ([a709cc88](https://github.com/linuxdeepin/dde-session-ui/commit/a709cc88bb2c12034b1574756209a3a661f273ee))
*   remove libpam0g from debian/control ([2b3171de](https://github.com/linuxdeepin/dde-session-ui/commit/2b3171deddea292e4c081bc701a15fe9e080a1bc))



<a name=""></a>
##  (2017-10-25)


#### Bug Fixes

*   shutdown contentwidget not init ([ee42e21d](https://github.com/linuxdeepin/dde-session-ui/commit/ee42e21d9b60bab01af389e0e492ce9bba255808))

#### Features

*   add finger verification timed out tip ([4640a828](https://github.com/linuxdeepin/dde-session-ui/commit/4640a828a32dafba79e333065562063d32007bd7))



