.class public Lcom/example/hookpms/PmsHookBinderInvocationHandler;
.super Ljava/lang/Object;
.source "PmsHookBinderInvocationHandler.java"

# interfaces
.implements Ljava/lang/reflect/InvocationHandler;


# static fields
.field public static final ZXA:Ljava/lang/String; = "zxa"


# instance fields
.field private SIGN:Ljava/lang/String;

.field private appPkgName:Ljava/lang/String;

.field private base:Ljava/lang/Object;


# direct methods
.method public constructor <init>(Ljava/lang/Object;Ljava/lang/String;Ljava/lang/String;I)V
    .registers 6
    .param p1, "base"    # Ljava/lang/Object;
    .param p2, "sign"    # Ljava/lang/String;
    .param p3, "appPkgName"    # Ljava/lang/String;
    .param p4, "hashCode"    # I

    .prologue
    .line 18
    invoke-direct {p0}, Ljava/lang/Object;-><init>()V

    .line 16
    const-string v0, ""

    iput-object v0, p0, Lcom/example/hookpms/PmsHookBinderInvocationHandler;->appPkgName:Ljava/lang/String;

    .line 20
    :try_start_7
    iput-object p1, p0, Lcom/example/hookpms/PmsHookBinderInvocationHandler;->base:Ljava/lang/Object;

    .line 21
    iput-object p2, p0, Lcom/example/hookpms/PmsHookBinderInvocationHandler;->SIGN:Ljava/lang/String;

    .line 22
    iput-object p3, p0, Lcom/example/hookpms/PmsHookBinderInvocationHandler;->appPkgName:Ljava/lang/String;
    :try_end_d
    .catch Ljava/lang/Exception; {:try_start_7 .. :try_end_d} :catch_e

    .line 27
    :goto_d
    return-void

    .line 24
    :catch_e
    move-exception v0

    goto :goto_d
.end method


# virtual methods
.method public invoke(Ljava/lang/Object;Ljava/lang/reflect/Method;[Ljava/lang/Object;)Ljava/lang/Object;
    .registers 11
    .param p1, "proxy"    # Ljava/lang/Object;
    .param p2, "method"    # Ljava/lang/reflect/Method;
    .param p3, "args"    # [Ljava/lang/Object;
    .annotation system Ldalvik/annotation/Throws;
        value = {
            Ljava/lang/Throwable;
        }
    .end annotation

    .prologue
    const/4 v6, 0x0

    .line 32
    const-string v4, "zxa"

    invoke-virtual {p2}, Ljava/lang/reflect/Method;->getName()Ljava/lang/String;

    move-result-object v5

    invoke-static {v4, v5}, Landroid/util/Log;->i(Ljava/lang/String;Ljava/lang/String;)I

    .line 35
    const-string v4, "getPackageInfo"

    invoke-virtual {p2}, Ljava/lang/reflect/Method;->getName()Ljava/lang/String;

    move-result-object v5

    invoke-virtual {v4, v5}, Ljava/lang/String;->equals(Ljava/lang/Object;)Z

    move-result v4

    if-eqz v4, :cond_4a

    .line 36
    aget-object v2, p3, v6

    check-cast v2, Ljava/lang/String;

    .line 37
    .local v2, "pkgName":Ljava/lang/String;
    const/4 v4, 0x1

    aget-object v0, p3, v4

    check-cast v0, Ljava/lang/Integer;

    .line 39
    .local v0, "flag":Ljava/lang/Integer;
    invoke-virtual {v0}, Ljava/lang/Integer;->intValue()I

    move-result v4

    const/16 v5, 0x40

    if-ne v4, v5, :cond_4a

    iget-object v4, p0, Lcom/example/hookpms/PmsHookBinderInvocationHandler;->appPkgName:Ljava/lang/String;

    invoke-virtual {v4, v2}, Ljava/lang/String;->equals(Ljava/lang/Object;)Z

    move-result v4

    if-eqz v4, :cond_4a

    .line 41
    new-instance v3, Landroid/content/pm/Signature;

    iget-object v4, p0, Lcom/example/hookpms/PmsHookBinderInvocationHandler;->SIGN:Ljava/lang/String;

    invoke-direct {v3, v4}, Landroid/content/pm/Signature;-><init>(Ljava/lang/String;)V

    .line 42
    .local v3, "sign":Landroid/content/pm/Signature;
    iget-object v4, p0, Lcom/example/hookpms/PmsHookBinderInvocationHandler;->base:Ljava/lang/Object;

    invoke-virtual {p2, v4, p3}, Ljava/lang/reflect/Method;->invoke(Ljava/lang/Object;[Ljava/lang/Object;)Ljava/lang/Object;

    move-result-object v1

    check-cast v1, Landroid/content/pm/PackageInfo;

    .line 43
    .local v1, "packageInfo":Landroid/content/pm/PackageInfo;
    iget-object v4, v1, Landroid/content/pm/PackageInfo;->signatures:[Landroid/content/pm/Signature;

    aput-object v3, v4, v6

    .line 44
    const-string v4, "zxa"

    const-string v5, "\u66ff\u6362\u7b7e\u540d\u5b8c\u6210"

    invoke-static {v4, v5}, Landroid/util/Log;->i(Ljava/lang/String;Ljava/lang/String;)I

    .line 50
    .end local v0    # "flag":Ljava/lang/Integer;
    .end local v1    # "packageInfo":Landroid/content/pm/PackageInfo;
    .end local v2    # "pkgName":Ljava/lang/String;
    .end local v3    # "sign":Landroid/content/pm/Signature;
    :goto_49
    return-object v1

    :cond_4a
    iget-object v4, p0, Lcom/example/hookpms/PmsHookBinderInvocationHandler;->base:Ljava/lang/Object;

    invoke-virtual {p2, v4, p3}, Ljava/lang/reflect/Method;->invoke(Ljava/lang/Object;[Ljava/lang/Object;)Ljava/lang/Object;

    move-result-object v1

    goto :goto_49
.end method
