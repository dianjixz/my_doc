UnitV2Framework : https://github.com/m5stack/UnitV2Framework.git
M5Unit-V2-AICamera: https://github.com/m5stack/M5Unit-V2-AICamera.git


unitv2 的程序会输出检测结果和图片,不过图片的输出需要先输入参数:
_{"stream":1} 进行视频流开启.




time:2024年 01月 19日 星期五 11:26:01 CST
# 记一次 unitv2 的训练服务维修记录
这次的其因是因为 unitv2的训练服务又被人投诉了，在之前的时间中，我只是检查了训练服务的运行状态，并没有对训练的结果进行检查。
显然，对训练结果检查是一个非常烦人的操作，而且这并不是我的主要工作。
客服说用户在 VTrainingService 训练模型时，一直出现训练错误的现象。
然后我检查了后台程序的运行状况，我发现这个训练框架每隔 2 个小时就会被重启一次。其实这个在很早就发现了，由于懒惰，况且并没有投诉记录，所以我并不想管这个看起来就是错误的设定。
初步检查出问题后，我就把这个重启给关掉了。然后测试了训练任务，当然它工作的很好。过了不到一周后，用户开始反馈，这次的训练一直处于训练中没有结果。重启程序后，训练服务就恢复了正常，但经过一段时间后我再次检查时，发现了它又失效了。这时我就觉察到了不对，也许刚开始的时定时两个小时重启是故意的，代码中应该有 2 个小时掉线的任务。检查了一下，果然如此，于是快乐的修复了这个 bug，服务器重新上线，一切都是这么的愉快。
经过了两天的运行，我发现了一个令人震惊的错误，显卡竟然掉了。说实话，面对这个情况我真的是慌了。检查了环境后发现，仅仅是 docker 中的显卡挂掉了。此时我并不知道为什么会发生这样的情况，看着屎一样的服务代码，我放弃继续修复这个屎山了，鬼知道这个代码还有哪些会发生错误的地方。好的这个服务框架并不复杂，我很快就就能重构它。在周会上我提出了重构这个服务框架的本周任务。
重构任务进行的很快，用了一天半的时间就成功的对代码进行了重构。我对自己的代码功底还是很自信的。
事实说明，重构是苦难的开始。
服务开始运行，模型也正常产出了， 我通知了公司的一个在外测试测试这个服务。服务运行的很好，然后我就让他测试了一下产出的模型。模型并没有正常工作，没有检测出任何东西。我炸了，最不想看到的事情终于发生了。训练代码出问题了，训练代码是和 AI 模型有关系的，需要一个 AI 工程师来解决它。公司并没有 AI 工程师，这个工作对于我来说还是有些困难的，但是战胜困难才会成长，况且，我也正在从事业余的 AI 研究工作。
我检查了模型，发现训练框架产出的模型和设备上运行的模型不匹配，一番搜索后，nanodet 的作者升级了模型，新的模型更改了模型的检测头。这是一个最不想看到的结果，因为这个真的很难，或者说几乎无法解决。
前人留下的屎，哭着也要吃完。
经过了一天的努力，降版本，无法运行；降环境，无法运行；现在只剩下最后一个选项，将模型再改回去，这是一项非常痛苦的工作。一天过后，我给商务部发了服务器下线维护的通知，并给了 2 周的预期时间。期望在这两周的时间内能将模型改好。
第二天，梳理了训练框架的主要结构，开始上手更改检测头。我将模型头部代码恢复到了以前的样子，刚开始还算顺利。当更改工作完成后，它成功的训练起来的一个流程。验证过程出问题了。
验证的时候，模型的一个中间张量没有完成预期的代码处理。经过了更加仔细数次的减检查，问题依旧：
```bash
../aten/src/ATen/native/cuda/IndexKernel.cu:92: operator(): block: [193,0,0], thread: [96,0,0] Assertion `index >= -sizes[i] && index < sizes[i] && "index out of bounds"` failed.
```
刚开始还以为是我的问题，一番努力后，决定在网上搜索一下这个错误。网上的说法是，可能是 torch 版本的问题。配置环境是一个非常糟心的事情，要将数个软件包的版本对应起来是一个非常头疼的工作，最重要的是，训练框架中的软件依赖包并不准确。尝试了几次后，感觉有些绝望。
但最终，经过努力还是成功的将模型的问题修复了。
torch 在版本迭代过程中会改变某种情况下的张量操作，猜测是这个原因导致了上面错误的发生，虽然服务运行了起来，如果环境不发生改变的话，基本没什么问题。如果下次再更新硬件，只能期望下一个维护者运气够好吧。期望不是我。
唉！努力和回报总是不成正比的，工作向来如此。努力修好了只是应该做的，但是出错了，锅一定是我的，是谁绑架了我们，又是谁造成了这样。这一切都无从查起，也许世界就是一个草台班子。


















