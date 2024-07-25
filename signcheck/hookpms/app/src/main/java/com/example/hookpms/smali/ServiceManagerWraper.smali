.class public Lcom/example/hookpms/ServiceManagerWraper;
.super Ljava/lang/Object;
.source "ServiceManagerWraper.java"


# static fields
.field public static final ZXA:Ljava/lang/String; = "zxa"


# direct methods
.method public constructor <init>()V
    .registers 1

    .prologue
    .line 11
    invoke-direct {p0}, Ljava/lang/Object;-><init>()V

    return-void
.end method

.method public static hookPMS(Landroid/content/Context;)V
    .registers 4
    .param p0, "context"    # Landroid/content/Context;

    .prologue
    .line 51
    const-string v0, "com.example.signcheck01"

    .line 52
    .local v0, "appPkgName":Ljava/lang/String;
    const-string v1, "30820253308201bca00302010202044bbb0361300d06092a864886f70d0101050500306d310e300c060355040613054368696e61310f300d06035504080c06e58c97e4baac310f300d06035504070c06e58c97e4baac310f300d060355040a0c06e885bee8aeaf311b3019060355040b0c12e697a0e7babfe4b89ae58aa1e7b3bbe7bb9f310b30090603550403130251513020170d3130303430363039343831375a180f32323834303132303039343831375a306d310e300c060355040613054368696e61310f300d06035504080c06e58c97e4baac310f300d06035504070c06e58c97e4baac310f300d060355040a0c06e885bee8aeaf311b3019060355040b0c12e697a0e7babfe4b89ae58aa1e7b3bbe7bb9f310b300906035504031302515130819f300d06092a864886f70d010101050003818d0030818902818100a15e9756216f694c5915e0b529095254367c4e64faeff07ae13488d946615a58ddc31a415f717d019edc6d30b9603d3e2a7b3de0ab7e0cf52dfee39373bc472fa997027d798d59f81d525a69ecf156e885fd1e2790924386b2230cc90e3b7adc95603ddcf4c40bdc72f22db0f216a99c371d3bf89cba6578c60699e8a0d536950203010001300d06092a864886f70d01010505000381810094a9b80e80691645dd42d6611775a855f71bcd4d77cb60a8e29404035a5e00b21bcc5d4a562482126bd91b6b0e50709377ceb9ef8c2efd12cc8b16afd9a159f350bb270b14204ff065d843832720702e28b41491fbc3a205f5f2f42526d67f17614d8a974de6487b2c866efede3b4e49a0f916baa3c1336fd2ee1b1629652049"

    .line 53
    .local v1, "sign":Ljava/lang/String;
    const/4 v2, 0x0

    invoke-static {p0, v1, v0, v2}, Lcom/example/hookpms/ServiceManagerWraper;->hookPMS(Landroid/content/Context;Ljava/lang/String;Ljava/lang/String;I)V

    .line 54
    return-void
.end method

.method public static hookPMS(Landroid/content/Context;Ljava/lang/String;Ljava/lang/String;I)V
    .registers 17
    .param p0, "context"    # Landroid/content/Context;
    .param p1, "signed"    # Ljava/lang/String;
    .param p2, "appPkgName"    # Ljava/lang/String;
    .param p3, "hashCode"    # I

    .prologue
    .line 20
    :try_start_0
    const-string v9, "android.app.ActivityThread"

    invoke-static {v9}, Ljava/lang/Class;->forName(Ljava/lang/String;)Ljava/lang/Class;

    move-result-object v0

    .line 21
    .local v0, "activityThreadClazz":Ljava/lang/Class;, "Ljava/lang/Class<*>;"
    const-string v9, "currentActivityThread"

    const/4 v10, 0x0

    new-array v10, v10, [Ljava/lang/Class;

    invoke-virtual {v0, v9, v10}, Ljava/lang/Class;->getDeclaredMethod(Ljava/lang/String;[Ljava/lang/Class;)Ljava/lang/reflect/Method;

    move-result-object v2

    .line 22
    .local v2, "currentActivityThreadMethod":Ljava/lang/reflect/Method;
    const/4 v9, 0x0

    const/4 v10, 0x0

    new-array v10, v10, [Ljava/lang/Object;

    invoke-virtual {v2, v9, v10}, Ljava/lang/reflect/Method;->invoke(Ljava/lang/Object;[Ljava/lang/Object;)Ljava/lang/Object;

    move-result-object v1

    .line 25
    .local v1, "currentActivityThrea":Ljava/lang/Object;
    const-string v9, "sPackageManager"

    invoke-virtual {v0, v9}, Ljava/lang/Class;->getDeclaredField(Ljava/lang/String;)Ljava/lang/reflect/Field;

    move-result-object v8

    .line 26
    .local v8, "sPackageManagerField":Ljava/lang/reflect/Field;
    const/4 v9, 0x1

    invoke-virtual {v8, v9}, Ljava/lang/reflect/Field;->setAccessible(Z)V

    .line 27
    invoke-virtual {v8, v0}, Ljava/lang/reflect/Field;->get(Ljava/lang/Object;)Ljava/lang/Object;

    move-result-object v7

    .line 30
    .local v7, "sPackageManager":Ljava/lang/Object;
    const-string v9, "android.content.pm.IPackageManager"

    invoke-static {v9}, Ljava/lang/Class;->forName(Ljava/lang/String;)Ljava/lang/Class;

    move-result-object v3

    .line 31
    .local v3, "iPackageManagerInterface":Ljava/lang/Class;, "Ljava/lang/Class<*>;"
    invoke-virtual {v3}, Ljava/lang/Class;->getClassLoader()Ljava/lang/ClassLoader;

    move-result-object v9

    const/4 v10, 0x1

    new-array v10, v10, [Ljava/lang/Class;

    const/4 v11, 0x0

    aput-object v3, v10, v11

    new-instance v11, Lcom/example/hookpms/PmsHookBinderInvocationHandler;

    const/4 v12, 0x0

    invoke-direct {v11, v7, p1, p2, v12}, Lcom/example/hookpms/PmsHookBinderInvocationHandler;-><init>(Ljava/lang/Object;Ljava/lang/String;Ljava/lang/String;I)V

    invoke-static {v9, v10, v11}, Ljava/lang/reflect/Proxy;->newProxyInstance(Ljava/lang/ClassLoader;[Ljava/lang/Class;Ljava/lang/reflect/InvocationHandler;)Ljava/lang/Object;

    move-result-object v6

    .line 36
    .local v6, "proxy":Ljava/lang/Object;
    invoke-virtual {v8, v1, v6}, Ljava/lang/reflect/Field;->set(Ljava/lang/Object;Ljava/lang/Object;)V

    .line 38
    invoke-virtual {p0}, Landroid/content/Context;->getPackageManager()Landroid/content/pm/PackageManager;

    move-result-object v5

    .line 39
    .local v5, "pm":Landroid/content/pm/PackageManager;
    invoke-virtual {v5}, Ljava/lang/Object;->getClass()Ljava/lang/Class;

    move-result-object v9

    const-string v10, "mPM"

    invoke-virtual {v9, v10}, Ljava/lang/Class;->getDeclaredField(Ljava/lang/String;)Ljava/lang/reflect/Field;

    move-result-object v4

    .line 41
    .local v4, "mPmField":Ljava/lang/reflect/Field;
    const/4 v9, 0x1

    invoke-virtual {v4, v9}, Ljava/lang/reflect/Field;->setAccessible(Z)V

    .line 42
    invoke-virtual {v4, v5, v6}, Ljava/lang/reflect/Field;->set(Ljava/lang/Object;Ljava/lang/Object;)V

    .line 43
    const-string v9, "zxa"

    const-string v10, "hook pms done "

    invoke-static {v9, v10}, Landroid/util/Log;->i(Ljava/lang/String;Ljava/lang/String;)I
    :try_end_5e
    .catch Ljava/lang/Exception; {:try_start_0 .. :try_end_5e} :catch_5f

    .line 47
    .end local v0    # "activityThreadClazz":Ljava/lang/Class;, "Ljava/lang/Class<*>;"
    .end local v1    # "currentActivityThrea":Ljava/lang/Object;
    .end local v2    # "currentActivityThreadMethod":Ljava/lang/reflect/Method;
    .end local v3    # "iPackageManagerInterface":Ljava/lang/Class;, "Ljava/lang/Class<*>;"
    .end local v4    # "mPmField":Ljava/lang/reflect/Field;
    .end local v5    # "pm":Landroid/content/pm/PackageManager;
    .end local v6    # "proxy":Ljava/lang/Object;
    .end local v7    # "sPackageManager":Ljava/lang/Object;
    .end local v8    # "sPackageManagerField":Ljava/lang/reflect/Field;
    :goto_5e
    return-void

    .line 44
    :catch_5f
    move-exception v9

    goto :goto_5e
.end method
